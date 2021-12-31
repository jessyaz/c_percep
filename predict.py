from skimage.transform import resize
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

import tensorflow as tf
from keras.models import load_model


def display_image(X, y, n, label=False):
    plt.figure(figsize=(20,2))
    for i in range(1):
        ax = plt.subplot(1, n, i+1)
        plt.imshow(X.values[i].reshape(28,28))
        if label:
            plt.title("Digit: {}".format(y[i]))
        ax.get_xaxis().set_visible(False)
        ax.get_yaxis().set_visible(False)
    plt.show()



image = plt.imread('./bin/temp.bmp')

model = load_model('./src/model_cnn.h5')

image = resize(image, (28, 28, 4))
image.shape

print(image.shape)

np.array(image)
resp = image.reshape(784, 4)

matrice = np.arange(0.0, 784.0)

for i in range(0,len(resp)):
    matrice[i] = (1 - resp[i][0])
    #print(resp[i][0])

mat = np.array([matrice])

res = mat.reshape(-1, 28, 28, 1)

hat = model.predict(res, verbose=1)
label_reponse = np.argmax(hat, axis=1)

display_image(pd.DataFrame(res.reshape(-1, 784)), label_reponse, n=1, label=True)
print(hat)