import os
import numpy as np
import time
from sys import argv
from sklearn.model_selection import train_test_split
import argparse


# import matplotlib.pyplot as plt

def find_label_noise_pos(label):
    max = 0
    for i in range(label.size - 1):
        if (max < abs(label[i + 1] - label[i])):
            max = abs(label[i + 1] - label[i])
            pos = i + 1

    return pos, max


def ad_data_denoise(ad_array):
    rate = 0.8
    ad_new = []
    idx = 0
    oldvalue = ad_array[0]
    for i in range(ad_array.size):
        new = ad_array[i] * rate + (1 - rate) * oldvalue
        oldvalue = new
        ad_new.append(new)

    rt_array = np.array(ad_new)
    return rt_array


# extend array, continual 10 ad value
def ad_array_extension(ad_array, batch_size):
    ad_list = []
    list_len = ad_array.shape[0]
    for i in range(list_len):
        if (i > batch_size):
            ad_list.append([ad_array[i - batch_size:i]])
        else:
            ad_list.append([ad_array[0:batch_size]])
    ad_new = np.array(ad_list)

    return ad_new


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    # 这里主要实现从命令行取得需要读取的文件  默认读取文件为normal.txt
    parser.add_argument('-f', '--file', type=str, default='normal.txt',
                        help='txt file')

    parser.add_argument(
        '-b', '--batch_size', type=int, default=1,
        help="""\
           batch size
           """)
    # 解析命令行获取的参数
    args, unparsed = parser.parse_known_args()

    batch_size = args.batch_size
    # args.file需要打开的文件 dtype设置数据类型 delimiter分隔符 unpack是否每一列当做一个向量输出
    dat_str = np.loadtxt(args.file, dtype=np.str, delimiter=' ', unpack=False)

    print(dat_str)
    # 转换为一维数组
    dat_str = dat_str.reshape(dat_str.size)

    # 定义一个list
    dat_int = []
    # 遍历字符串读取每个数据
    for a in dat_str:
        if (len(a) > 0):
            #hex_str = '0x' + a  # 将数据转换为 0xXX
            dat_int.append(int(a, 16))  # 将数据转换为十进制并添加到dat_int列表

    # 将列表转换为数组便于使用
    dat_int = np.array(dat_int)
    # 计算有多少组完整数据
    len = int(int(len(dat_int) / 11) * 11)
    # 这是一步多余的操作
    dat_int = dat_int[0:len]

    # 将数据变形为矩阵 也可以说是二维数组
    dat_int = dat_int.reshape(int(dat_int.size / 11), 11)

    # 定义一个list
    list = []

    # 遍历dat_int矩阵
    for a in dat_int:
        if (a[10] != 90):
            print(a)
            raise ValueError('Error data')
            break
        # 计算时间
        time = (a[0] << 8) + a[1]
        # print('%d,%d,%d,%d' % (a[0], a[1], a[2],time))
        # 获取电感与PWM的数值
        l = a[2]
        ml = a[3]
        m = a[4]
        mr = a[5]
        r = a[6]
        fl = a[7]
        fr = a[8]
        pwm = a[9]
        # 将获取到的数值添加到list
        list.append([l, ml, m, mr, r, fl, fr, pwm])
    # 将list转换为数组 并将数据类型转换为有符号8位
    list = (np.array(list)).astype(np.int8)
    # 通过切片操作 获得ad数据
    ad_dat = list[:, [0, 1, 2, 3, 4, 5, 6]]
    # 通过切片操作 获得pwm数据
    pwm_dat = list[:, 7]
    # 新建一个ad数据
    dbg_ad = ad_dat
    # 通过切片操作读取每个通道的数值
    a_l = ad_data_denoise((list[:, 0]))
    a_ml = ad_data_denoise((list[:, 1]))
    a_m = ad_data_denoise((list[:, 2]))
    a_mr = ad_data_denoise((list[:, 3]))
    a_r = ad_data_denoise((list[:, 4]))
    a_fl = ad_data_denoise((list[:, 5]))
    a_fr = ad_data_denoise((list[:, 6]))
    pwm_dat = list[:, 7]
    # 转换数据类型为8位有符号数据 并通过ad_data_denoise函数滤波
    pwm_dat = ad_data_denoise(pwm_dat.astype('int8'))
    # 将切片对象按列连接
    ad_dat = np.c_[a_l, a_ml, a_m, a_mr, a_r, a_fl, a_fr]
    # 转换为有符号 不明白为啥要这样做 前面不是转换好了？
    ad_dat = ad_dat.astype('int8')

    # 扩展AD数据  实际上就是复制数据
    ad_dat = ad_array_extension(ad_dat, batch_size)
    # 将数据转换为三维数组 最外层与有多少组ad数据一致 中间层有参数指定 最内层为7个电感数据
    ad_dat = ad_dat.reshape(ad_dat.shape[0], batch_size, 7)

    origin_ad = ad_dat.astype('int8')
    origin_pwm = pwm_dat.astype('int8')

    # print(args.file)
    # 获取文件名称以及格式
    filename = args.file.split("\\")[-1]

    # print(filename)
    # 获取文件名
    filename = filename.split(".")[0]
    # print(filename)
    # 准备输出文件的名称
    pwm_fname = 'origin_pwm_' + filename + '.npy'
    ad_fname = 'origin_ad_' + filename + '.npy'
    np.save(pwm_fname, origin_pwm)
    np.save(ad_fname, origin_ad)

    print("Generate ad.npy shape:", ad_dat.shape)
    print("Generate pwm.npy shape:", pwm_dat.shape)
    print('ad data shape:%f~%f' % (max(origin_ad.flatten()), min(origin_ad.flatten())))
    print('pwm data shape:%f~%f' % (max(origin_pwm), min(origin_pwm)))

    print('Generate %s,%s' % (pwm_fname, ad_fname))
