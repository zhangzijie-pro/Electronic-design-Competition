# 用pytorch实现一个简单灰度图二分类模型

import torch
import torch.nn as nn
from model import Classify
import numpy as np
import os
import random

np.random.seed(0)

# 从txt读取数据
def get_data(path, cls_id, total_cls_id, x, y):
    # 遍历path文件夹下所有txt文件
    for root, dirs, file_names in os.walk(path):
        for file_name in file_names:
            if file_name.endswith('.txt'):
                file = os.path.join(root, file_name)
                with open(file, 'r', encoding='utf-8') as f:
                    lines = f.readlines()
                    # 把lines转换为int列表
                    data = []
                    for line in lines:
                        data.append([int(i) for i in line.strip().split()])
                    x.append(data)
                    cls_y = [0]*total_cls_id
                    cls_y[cls_id] = 1
                    y.append(cls_y)

# 生成数据
x = []
y = []
get_data('data/music', 0, 2, x, y)
get_data('data/voice', 1, 2, x, y)

x = np.array(x).reshape(-1, 1, 9)
y = np.array(y)

idx = np.random.permutation(x.shape[0])
x = x[idx]
y = y[idx]

train_x = x[:int(x.shape[0]*0.8)]
train_y = y[:int(y.shape[0]*0.8)]

val_x = x[int(x.shape[0]*0.8):]
val_y = y[int(y.shape[0]*0.8):]

test_idx = random.randint(0, len(val_x)-1)

# 读取模型
model = Classify()
model.load_state_dict(torch.load('model.pth'))

# 验证一张图片
model.eval()
predicted = model(torch.from_numpy(val_x[test_idx].astype(np.float32))).detach().numpy()
print('Predicted', predicted)
print('Ground Truth', val_y[0])

# 从txt读取权重
lines = []
with open('model.txt', 'r') as f:
    lines = f.readlines()

# 读取权重
weights = {}
for line in lines:
    key, values = line.strip().split(": ")
    # 将values里的所有[]去掉，然后按逗号分割
    values = values.replace('[', '').replace(']', '')
    values = values.split(", ")
    values = [float(i) for i in values]
    weights[key] = values

input = val_x[test_idx].astype(np.float32).squeeze().tolist()
print("input", input)

# conv1
conv1_weight = weights['conv1.weight']
conv1_bias = weights['conv1.bias']

conv1_output = []
for i in range(4):
    conv1 = 0
    for j in range(9):
        conv1 += input[j] * conv1_weight[i*9+j]
    conv1 += conv1_bias[i]
    conv1_output.append(conv1)

print("conv1", conv1_output)

# act
act_output = []
for i in range(4):
    if conv1_output[i] > 0:
        act_output.append(conv1_output[i])
    else:
        act_output.append(0)

print("act", act_output)

# conv2
conv2_weight = weights['conv2.weight']
conv2_bias = weights['conv2.bias']

conv2_output = []
for i in range(2):
    conv2 = 0
    for j in range(4):
        conv2 += act_output[j] * conv2_weight[i*4+j]
    conv2 += conv2_bias[i]
    conv2_output.append(conv2)

print("conv2", conv2_output)

# output
if conv2_output[0] > conv2_output[1]:
    print("music")
else:
    print("voice")
