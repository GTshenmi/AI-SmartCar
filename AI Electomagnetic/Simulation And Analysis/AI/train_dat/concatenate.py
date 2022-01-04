import os
import numpy as np
import time
from sys import argv
from sklearn.model_selection import train_test_split
import argparse


if __name__ == "__main__":
    path = "."
    ad_array = []
    pwm_array = []
    files = os.listdir(path)
    for file in files:

        if(file.split(".")[-1]  == 'npy'):
            if(file[0:9] == 'origin_ad'):
                ad = np.load(file)
                if(len(ad_array) == 0):
                    ad_array = ad
                else:
                    ad_array = np.concatenate((ad_array, ad))
            elif(file[0:10] == 'origin_pwm'):
                pwm = np.load(file)
                if (len(pwm_array) == 0):
                    pwm_array = pwm
                else:
                    pwm_array = np.concatenate((pwm_array, pwm))

    ad_dat = ad_array
    pwm_dat =pwm_array

    out_len = int(len(ad_dat) / 100) * 100
    test_ad_npy = ad_dat[0:out_len]
    label_pwm_npy = pwm_dat[0:out_len]
    rest_train = ad_dat[out_len:len(ad_dat)]
    rest_label = pwm_dat[out_len:len(ad_dat)]

    print(test_ad_npy.size, len(test_ad_npy))
    print(test_ad_npy.shape)


    train_data, test_data, train_label, test_label = train_test_split(test_ad_npy, label_pwm_npy, test_size=0.2,
                                                                      random_state=0)
    train_data = np.concatenate((train_data, rest_train))
    train_label = np.concatenate((train_label, rest_label))

    print('ad train data shape:%f~%f' % (max(train_data.flatten()), min(train_data.flatten())))
    print('pwm train data shape:%f~%f' % (max(train_label), min(train_label)))

    np.save('./ad_train_dat.npy', train_data)
    np.save('./ad_test_dat.npy', test_data)

    print('ad test data shape:%f~%f' % (max(test_data.flatten()), min(test_data.flatten())))
    print('pwm test data shape:%f~%f' % (max(test_label), min(test_label)))
    np.save('./pwm_train_label.npy', train_label)
    np.save('./pwm_test_label.npy', test_label)

    print('Generate ad_train_dat.npy,ad_test_dat.npy,pwm_train_label.npy,pwm_test_label.npy')
