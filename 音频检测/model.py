import torch
import torch.nn as nn

# 搭一个两层简单模型
class Classify(nn.Module):
    def __init__(self):
        super(Classify, self).__init__()
        self.conv1 = nn.Linear(9, 4, bias=True) # 3x3=9个输入值变换成4个输出值
        self.act = nn.ReLU() # 激活正值
        self.dropout = nn.Dropout(0.1) # 随机失活
        self.conv2 = nn.Linear(4, 2, bias=True) # 4个输入值变换成2个分类输出值
    
    def forward(self, x):
        if self.training:
            x = x + torch.randn_like(x)
        else:
            print("input", x)
        x = self.conv1(x)
        if not self.training:
            print("conv1", x)
        x = self.act(x)
        if not self.training:
            print("act", x)
        else:
            x = self.dropout(x)
        x = self.conv2(x).squeeze()
        if not self.training:
            print("conv2", x)
        x = torch.softmax(x, dim=-1)
        if not self.training:
            print("output", x)
        return x
