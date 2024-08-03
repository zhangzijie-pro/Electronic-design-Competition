# 用pytorch实现一个简单灰度图二分类模型

import torch
import torch.nn as nn
import torch.optim as optim
from model import Classify
import numpy as np
import os

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
# music 分类0
# voice 分类1
get_data('data/music', 0, 2, x, y)
get_data('data/voice', 1, 2, x, y)

# 输入数据格式为[batch, 1, 3, 3]
x = np.array(x).reshape(-1, 1, 9)
# 输出数据格式为[batch, 2]
y = np.array(y)

# 数据打乱
idx = np.random.permutation(x.shape[0])
x = x[idx]
y = y[idx]

# 划分训练集和验证集
train_x = x[:int(x.shape[0]*0.8)]
train_y = y[:int(y.shape[0]*0.8)]
val_x = x[int(x.shape[0]*0.8):]
val_y = y[int(y.shape[0]*0.8):]

model = Classify()

# 定义损失函数和优化器
criterion = nn.BCELoss()
optimizer = optim.AdamW(model.parameters(), lr=0.001)

# 训练模型
model.train()
num_epochs = 100
batch = 64

for epoch in range(num_epochs):
    assert train_x.shape[0] == train_y.shape[0]
    # 打乱数据
    idx = np.random.permutation(train_x.shape[0])
    train_x = train_x[idx]
    train_y = train_y[idx]
    # 每次取batch个数据遍历所有
    for i in range(train_x.shape[0]-batch+1):
        inputs = torch.from_numpy(train_x[i:i+batch].astype(np.float32))
        targets = torch.from_numpy(train_y[i:i+batch].astype(np.float32))
        
        # forward
        outputs = model(inputs)
        loss = criterion(outputs, targets)
        
        # 逐步减小lr
        if epoch > num_epochs*2/3:
            optimizer.param_groups[0]['lr'] = 0.0001
        elif epoch > num_epochs/3:
            optimizer.param_groups[0]['lr'] = 0.0005

        # backward 
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

    if (epoch+1) % 10 == 0:
        print('Epoch [{}/{}], Loss: {:.4f}'.format(epoch+1, num_epochs, loss.item()))

# 验证模型精度
predicted = model(torch.from_numpy(val_x.astype(np.float32))).detach().numpy()
predicted = np.round(predicted)
print('Accuracy: {:.2f}'.format((predicted == val_y).sum() / len(val_y[0]) / len(val_y)))

# 保存模型
torch.save(model.state_dict(), 'model.pth')

# 权重保存为txt
with open('model.txt', 'w') as f:
    for name,param in model.named_parameters():
        f.write(name + ": " + str(param.data.numpy().tolist()).replace('[', '').replace(']', '').replace(' ', '').strip())
        f.write('\n')
