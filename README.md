# Reconnaissance de chiffres manuscrits : interface graphique en C et réseau de neurones convolutif

**Projet universitaire 2021/2022 --- 2 ème année de licence**

Auteur : Jessy A. (GitHub : [@jessyaz](https://github.com/jessyaz))
Dépôt : <https://github.com/jessyaz/c_percep>

---

## Sommaire

1. [Introduction](#1-introduction)
2. [Objectifs](#2-objectifs)
3. [Architecture générale](#3-architecture-générale)
4. [Partie C : interface et acquisition](#4-partie-c--interface-et-acquisition)
5. [Partie Python : classification par CNN](#5-partie-python--classification-par-cnn)
6. [Organisation du dépôt](#6-organisation-du-dépôt)
7. [Installation et exécution](#7-installation-et-exécution)
8. [Limites et perspectives](#8-limites-et-perspectives)
9. [Conclusion](#9-conclusion)
10. [Références](#10-références)

---

## 1. Introduction

La reconnaissance de chiffres manuscrits est un problème classique de la vision par ordinateur. Elle a été popularisée par les travaux de Yann LeCun sur les réseaux de neurones convolutifs (CNN) et par la base de données MNIST, qui contient des images de chiffres de taille 28 × 28 pixels réparties en 10 classes.

Ce projet propose une application complète de bout en bout : l'utilisateur dessine un chiffre à la souris dans une fenêtre, puis le programme lui indique quel chiffre a été reconnu. L'originalité du travail tient à la répartition des rôles entre deux langages :

- le **langage C**, avec la bibliothèque graphique **SDL2**, prend en charge l'interface, le tracé du dessin et l'acquisition de l'image ;
- le **langage Python**, avec **Keras**, prend en charge le prétraitement de l'image et la classification par un CNN pré-entraîné.

## 2. Objectifs

- Concevoir en C une interface de dessin interactive, sans bibliothèque de dessin de haut niveau : gestion des évènements, tracé continu et boutons.
- Implémenter un algorithme de tracé de segments afin d'obtenir un trait continu quelle que soit la vitesse de la souris.
- Interfacer un programme C avec un script Python pour exploiter un modèle d'apprentissage profond.
- Adapter l'image dessinée au format attendu par le réseau (28 × 28, un canal, valeurs normalisées).

## 3. Architecture générale

Le traitement se déroule en quatre étapes successives.

```
 ┌────────────────────────────┐
 │ 1. Dessin (C / SDL2)       │  évènements souris, tracé de segments
 └─────────────┬──────────────┘
               ▼
 ┌────────────────────────────┐
 │ 2. Acquisition (C / SDL2)  │  lecture du rendu → bin/temp.bmp
 │                            │  (+ export CSV → bin/temp.csv)
 └─────────────┬──────────────┘
               ▼   appel système : python ./src/predict.py
 ┌────────────────────────────┐
 │ 3. Prétraitement (Python)  │  redimensionnement 28×28, inversion
 └─────────────┬──────────────┘
               ▼
 ┌────────────────────────────┐
 │ 4. Classification (Keras)  │  CNN → vecteur de 10 probabilités
 └────────────────────────────┘
```

La communication entre les deux parties se fait par **fichier** : le programme C enregistre l'image dans `bin/temp.bmp`, puis lance le script Python par la fonction `system()` de la bibliothèque standard. Le script relit ce fichier.

## 4. Partie C : interface et acquisition

### 4.1 Interface graphique (`main.c`, `init_sdl.c`)

La fenêtre SDL2 (468 × 530 px) est composée d'un fond chargé depuis `src/images/backg.bmp`. Il définit une zone de dessin (abscisses inférieures à 336 px) et un panneau de commandes à droite. Le programme repose sur une boucle d'évènements bloquante (`SDL_WaitEvent`) qui traite :

- le **mouvement de la souris** avec le bouton gauche enfoncé, qui déclenche le dessin ;
- le **relâchement du bouton**, qui interrompt le trait afin de ne pas relier deux tracés distincts ;
- les **zones cliquables** qui jouent le rôle de boutons : *crayon* (noir), *gomme* (blanc) et *valider* ;
- la touche **Échap** et l'évènement `SDL_QUIT`, qui terminent le programme.

Le fichier `init_sdl.c` regroupe l'initialisation, les tests de bon fonctionnement de chaque composant SDL (fenêtre, rendu, surface, texture) et la libération de la mémoire en cas d'erreur ou en fin d'exécution.

### 4.2 Tracé de segments (`trace_segment.c`)

La capture de la souris est discrète : lorsque le curseur se déplace rapidement, deux positions successives peuvent être éloignées de plusieurs pixels, ce qui laisse des trous dans le trait. La fonction `trace_line` relie donc deux points A et B par un segment, en n'utilisant que des opérations sur les entiers, selon le principe de l'**algorithme de Bresenham**.

Soient `dx = xB − xA` et `dy = yB − yA`. Le tracé distingue plusieurs cas :

| Cas | Traitement |
|---|---|
| `dx = 0` | segment vertical, incrément de `y` uniquement |
| `dy = 0` | segment horizontal, incrément de `x` uniquement |
| `|dx| = |dy|` | diagonale, incrément simultané de `x` et `y` |
| `|dx| > |dy|` | pente douce : `x` avance à chaque pas, `y` avance lorsque le cumul de l'erreur `reste` atteint `|dx|` |
| `|dx| < |dy|` | pente forte : rôles de `x` et `y` échangés |

À chaque pas, un carré de 10 × 10 pixels est dessiné, ce qui donne au trait une épaisseur suffisante pour qu'il reste lisible après réduction à 28 × 28.

### 4.3 Acquisition de l'image (`validation.c`)

Au clic sur *Valider* :

1. `save_target_renderer` lit les pixels du rendu avec `SDL_RenderReadPixels` et sauvegarde l'image dans `bin/temp.bmp` ;
2. `load_pic` recharge cette image et parcourt tous ses pixels avec la fonction `getPixel`, qui gère les formats de 1 à 4 octets par pixel et l'ordre des octets (little/big endian). Chaque pixel est converti en niveau de gris par la moyenne `(R + G + B) / 3`, puis les valeurs sont écrites à plat dans `bin/temp.csv`. Cet export a été prévu pour une autre stratégie de traitement des données et n'est pas utilisé par le script de prédiction actuel ;
3. le script Python est ensuite lancé par `system("python ./src/predict.py")`.

## 5. Partie Python : classification par CNN

### 5.1 Prétraitement (`predict.py`)

Le script effectue les opérations suivantes :

1. lecture de `bin/temp.bmp` ;
2. redimensionnement de l'image à 28 × 28 pixels avec `skimage.transform.resize` (ce qui ramène également les valeurs dans l'intervalle [0, 1]) ;
3. extraction d'un canal de couleur et **inversion** (`1 − valeur`) : le dessin est noir sur fond blanc, alors que les images MNIST sont blanches sur fond noir ;
4. mise en forme du tenseur d'entrée en `(1, 28, 28, 1)`.

### 5.2 Architecture du réseau

Le modèle pré-entraîné est stocké dans `src/model_cnn.h5` (format Keras/HDF5). Son architecture, relevée dans le fichier, est la suivante :

| Étage | Couches |
|---|---|
| Extraction de caractéristiques 1 | 2 × Conv2D (32 filtres 5×5, ReLU), chacune suivie d'une BatchNormalization ; MaxPooling 2×2 ; Dropout 0,25 |
| Extraction de caractéristiques 2 | 2 × Conv2D (64 filtres 3×3, ReLU), chacune suivie d'une BatchNormalization ; MaxPooling 2×2 ; Dropout 0,25 |
| Classification | Flatten ; Dense 1024 (ReLU) ; Dropout 0,25 ; Dense 1024 (ReLU) ; Dropout 0,25 ; Dense 10 (softmax) |

Les couches convolutives extraient des motifs locaux (traits, courbes, boucles), les couches de *pooling* réduisent la résolution spatiale, la *batch normalization* stabilise l'apprentissage et le *dropout* limite le surapprentissage. La couche finale *softmax* fournit une distribution de probabilité sur les chiffres de 0 à 9.

### 5.3 Décision et affichage

Le chiffre prédit est celui de probabilité maximale (`argmax` du vecteur de sortie). Le script affiche le vecteur de probabilités dans la console et présente, avec `matplotlib`, l'image 28 × 28 effectivement fournie au réseau, avec le chiffre prédit en titre. Cela permet de contrôler visuellement la qualité du prétraitement.

## 6. Organisation du dépôt

```
c_percep/
├── makefile              # compilation et lancement
├── bin/                  # fichiers générés (temp.bmp, temp.csv, run.out)
└── src/
    ├── main.c            # fenêtre, boucle d'évènements, dessin, boutons
    ├── init_sdl.c / .h   # initialisation, tests et libération des ressources SDL
    ├── trace_segment.c   # algorithme de tracé de segments
    ├── validation.c / .h # acquisition de l'image, export CSV, appel de Python
    ├── predict.py        # prétraitement et prédiction
    ├── model_cnn.h5      # modèle CNN pré-entraîné
    └── images/backg.bmp  # arrière-plan de l'interface
```

## 7. Installation et exécution

### Prérequis

- compilateur C (`gcc`) et `make` ;
- bibliothèque **SDL2** et ses fichiers d'en-tête ;
- **Python 3** avec les paquets `tensorflow` (Keras), `numpy`, `scikit-image` et `matplotlib`.

```bash
# Debian / Ubuntu
sudo apt install build-essential libsdl2-dev

# macOS (Homebrew)
brew install sdl2

# Dépendances Python
pip install tensorflow numpy scikit-image matplotlib
```

### Compilation et lancement

Les chemins utilisés par le programme sont relatifs : la commande doit être lancée **depuis la racine du dépôt**.

```bash
git clone https://github.com/jessyaz/c_percep.git
cd c_percep
make
```

Le `makefile` compile les sources vers `bin/run.out`, exécute le programme, puis nettoie les fichiers générés à la fermeture.

> **Remarque.** Le chemin d'inclusion de SDL2 du `makefile` (`/usr/local/opt/sdl2/include/SDL2/`) correspond à une installation par Homebrew. Il doit être adapté à la configuration de la machine, par exemple `/usr/include/SDL2/` sous Linux. Le dossier `bin/` doit également être accessible en écriture.

### Mode d'emploi

1. Dessiner un chiffre avec le bouton gauche de la souris dans la zone de gauche.
2. Corriger si nécessaire avec la gomme, puis revenir au crayon.
3. Cliquer sur **Valider**.
4. Lire le résultat dans la fenêtre `matplotlib` (chiffre prédit et image 28 × 28) et dans la console (vecteur de probabilités).

## 8. Limites et perspectives

**Limites observées**

- Le prétraitement se limite à un redimensionnement et à une inversion. Il n'y a pas de recentrage ni de normalisation de la taille du chiffre comme dans MNIST, ce qui peut dégrader la reconnaissance.
- Le trait, épais (10 px), diffère du tracé des données d'apprentissage.
- La communication par fichier temporaire et par `system()` est simple mais peu robuste et coûteuse : le modèle est rechargé à chaque validation.
- Le résultat s'affiche dans une fenêtre distincte de l'interface.

**Perspectives**

- Recadrer le chiffre sur sa boîte englobante et le centrer dans l'image 28 × 28 avant la prédiction.
- Ajouter un lissage du trait et un bouton d'effacement complet.
- Remplacer l'appel système par une communication plus directe (tube, socket ou intégration de l'interpréteur Python dans le programme C), ou maintenir le modèle en mémoire.
- Afficher la prédiction et les probabilités directement dans la fenêtre SDL.
- Évaluer quantitativement les performances du modèle sur un jeu de test.

## 9. Conclusion

Ce projet a permis de réaliser une chaîne complète de reconnaissance de chiffres, de la saisie graphique jusqu'à l'inférence d'un réseau de neurones. Il a mobilisé la programmation système en C (gestion des évènements, mémoire, fichiers, algorithme de tracé) et l'exploitation d'un modèle d'apprentissage profond en Python. Il met aussi en évidence l'importance du prétraitement : les performances d'un classifieur dépendent autant de la qualité des données qui lui sont fournies que de son architecture.

## 10. Références

- LeCun, Y., Bottou, L., Bengio, Y., Haffner, P. (1998). *Gradient-based learning applied to document recognition*. Proceedings of the IEEE, 86(11), 2278-2324.
- LeCun, Y., Cortes, C., Burges, C. *The MNIST database of handwritten digits*. <http://yann.lecun.com/exdb/mnist/>
- Bresenham, J. E. (1965). *Algorithm for computer control of a digital plotter*. IBM Systems Journal, 4(1), 25-30.
- Documentation de SDL2 : <https://wiki.libsdl.org/>
- Documentation de Keras : <https://keras.io/>
