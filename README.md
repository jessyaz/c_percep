# c_percep

c_percep est un programme de dessin et de reconnaissance de chiffre.
c_percep est un projet d'études de deuxième années.

# ATTENTION

-> SDL incompatible avec les processeur silicon M1. (ou utiliser rosetta)

# INSTALLATION

- Avoir le compilateur gcc.

- Avoir installer la librairie SDL2 correctement pour une utilisation avec gcc.

- Avoir installé python et les librairies ci-dessous.

Si anaconda est déjà installer ne pas suivre les flèches suivantes et passer à l'execution. (anaconda possède initialement ces librairies)

-> installation de scikit-image librairie
    # Update pip
    python -m pip install -U pip
    # Install scikit-image
    python -m pip install -U scikit-image

-> installation de tensorflow
    #Install tensorflow
    pip install --upgrade tensorflow

-> installation de matplotlib
    python -m pip install -U matplotlib
    
    
    
- Effectuer un git clone du repo grâce à la commande : git clone https://github.com/jessyaz/c_percep/



#EXECUTION

- Dans la racine du repo cloné, éxecuter la commande   make   à l'aide du terminal.

Pour dessiner, il existe le crayon noir, ou la gomme blanche, à selectionner à l'aide la souris.

Un fois le dessin effectué, Valider puis patienter quelques secondes.

La touche echap permet de quitter l'application principale.
