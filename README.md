## 电赛G题源代码内容————声音屏蔽器

该仓库为stm32f407固件库内容编写的电赛G题声音屏蔽器,freertos分支为FreeRTOS的示例教程内容

#### 使用keil或者VsCode

## 文件格式
| 目录名                 | 描述                     |
|-----------------------|--------------------------|
| DSP/ CORE/ FWLIB/ SYSTEM/                 | stm32标准库文件                       |
| HARDWARE/ driver/              | 外设文件                            |
| USER/          | main文件                 |
| include/                 | 头文件                          |
| 音频检测/        | [Python用FFT输出的TOP3频率序号分类音乐和语音](./音频检测/Readme.md)  |

## 性能
角度: +-60°(120°)

距离: 1.5m+-0.5m

## 功能:
  1.可识别声音为人声或者纯音乐
  
  2.可选择性屏蔽声音类别
  
  3.拥有自动识别声音选择性屏蔽
  
  4.对应9V供电,可控制输入功率为<=6W,输出功率在1-4W(起步2W,1W不支持运行)

## 相关教程,说明
- [汇编教程1](https://dosasm.github.io/docs/notes-njupt/%E6%B1%87%E7%BC%96%E8%AF%AD%E8%A8%80%E6%8C%87%E4%BB%A4%E9%9B%86)
- [汇编教程2](https://www.w3cschool.cn/assembly/assembly-stack.html)
- [STM32F4系列教程](https://dengjin.blog.csdn.net/article/details/108050011)
- 对于训练Python的模型可通过adc采集1024组数据,进行FFT后,将得到FFT的结果进行排序得到最高的5个峰值,每5个一组放入到python中进行训练模型参数,放入DSP中的rfft函数中进行切换你采集的数据(数据越多更精准，由于stm32的算力不支持太大)
