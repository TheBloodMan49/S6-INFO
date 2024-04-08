#!/usr/bin/python3

# assert Tensorflow don't reserve all gpu memory
import tensorflow as tf
from keras.src.layers import Flatten

gpus = tf.config.experimental.list_physical_devices('GPU')
if gpus:
    try:
        for gpu in gpus:
            tf.config.experimental.set_memory_growth(gpu, True)
    except RuntimeError as e:
        print(e)

import numpy as np
from sklearn.datasets import load_svmlight_file, dump_svmlight_file
from sklearn.metrics import classification_report, accuracy_score


def perceptron(nbhidden):
    entree = tf.keras.Input(shape=(3072,))
    hidden = tf.keras.layers.Dense(units=nbhidden[0], activation='relu')(entree)
    hidden2 = tf.keras.layers.Dense(units=nbhidden[1], activation='relu')(hidden)
    hidden3 = tf.keras.layers.Dense(units=nbhidden[2], activation='relu')(hidden2)
    sortie = tf.keras.layers.Dense(units=10, activation='softmax')(hidden3)

    model = tf.keras.Model(inputs=entree, outputs=sortie)
    model.compile(loss='sparse_categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
    model.summary()
    return model

def cnn():
    entree = tf.keras.Input(shape=(32, 32, 3))
    layer = tf.keras.layers.Conv2D(32, (3, 3), activation='relu')(entree)
    layer = tf.keras.layers.MaxPooling2D(2)(layer)
    layer2 = tf.keras.layers.Conv2D(32, (3, 3), activation='relu')(layer)
    layer2 = tf.keras.layers.MaxPooling2D(2)(layer2)
    flatten = Flatten()(layer2)
    layer3 = tf.keras.layers.Dense(64, activation='relu')(flatten)
    out = tf.keras.layers.Dense(10, activation='softmax')(layer3)
    model = tf.keras.Model(inputs=entree, outputs=out)
    model.compile(loss='sparse_categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
    print(model.summary())
    return model

def main():
    # x_train, x_test: 3*uint8 array of RGB image data with shape (num_samples, 32, 32,3).
    # y_train, y_test: uint8 array of digit labels (integers in range 0-9) with shape (num_samples,).
    (x_train, y_train), (x_test, y_test) = tf.keras.datasets.cifar10.load_data()
    print('x_train.shape=', x_train.shape)
    print('x_test.shape=', x_test.shape)
    # pour preceptron, on represente chaque image par un vecteur de 3072 pixel
    # x_train = x_train.reshape(50000, 3072) / 255
    # x_test = x_test.reshape(10000, 3072) / 255
    # clf = perceptron([512, 256, 32])

    # pour CNN, on represente chaque image par un tenseur de 32*32 pixels codé sur 3 octets
    x_train = x_train.reshape(50000,32,32,3)
    x_test = x_test.reshape(10000,32,32,3)
    clf = cnn()

    clf.fit(x_train, y_train, epochs=20, batch_size=256)

    predictions = clf.predict(x_test).argmax(-1)  # le argmax sert à repérer le neurone actif et à donner son numéro

    print(classification_report(y_test, predictions))  # détails des erreurs
    print('accuracy_score=', accuracy_score(y_test, predictions))  # score à maximiser (sans tricher)


if __name__ == "__main__":
    main()
