from skimage.transform import resize
import matplotlib.pyplot as plt
import numpy as np
from keras.models import load_model
#from '../keraslib.models' import load_model

image = plt.imread('./bin/temp.bmp') #Lecture de l'image enregistré par le programme C.
model = load_model('./src/model_cnn.h5') #Chargement du modele de machine learning.
image = resize(image, (28, 28, 4)) #Redimenssionnement de l'image en 28 pixels par 28 pixels avec skimage.
#np.array(image) #On transcrit l'image sous forme de tableau numPy
resp = image.reshape(784, 4) #On redimensionne image en dimension (784, 4) avec numPy.
matrice = np.arange(0.0, 784.0) #On crée un tableau de la même taille que resp.
for i in range(0,len(resp)):
    matrice[i] = (1 - resp[i][0]) #On inverse les couleurs de resp.
mat = np.array([matrice]) #On stock les valeurs inversé dans le tableau mat.
res = mat.reshape(-1, 28, 28, 1) #On modifie les dimension de mat en (1, 28, 28, 1). Soit 1 image de 28 lignes contenant chacunes 28 colonnes ave des valeurs scalaire compris entre 0 et 1.
hat = model.predict(res, verbose=1) #On execute la commande de prediction et on injecte le tableau res.
label_reponse = np.argmax(hat, axis=1) #On recupère la valeur la plus haute du tableau hat.
print("Vecteur réponse : ", hat) #On affiche le vecteur hat (réponse).
plt.figure(figsize=(20,2)) #On crée une figure
plt.title("Digit: {}".format(label_reponse)) #On affiche un label au dessus de la figure.
plt.imshow(res.reshape(28,28)) #On reshape le tableau res pour pouvoir l'afficher sous forme d'image.
plt.show() #On affiche le graph de l'image.