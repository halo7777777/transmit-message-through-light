#### Opencv简介

>   OpenCV是一个基于BSD许可（开源）发行的跨平台计算机视觉库，可以运行在Linux、Windows、Android和Mac OS操作系统上。它轻量级而且高效——由一系列 C 函数和少量 C++ 类构成，同时提供了Python、Ruby、MATLAB等语言的接口，实现了图像处理和计算机视觉方面的很多通用算法。

#### VS+Opencv配置方法

> 1.opencv下载
> 2.环境变量更改
> 3.vs项目中的配置
> 具体见：[配置教程](https://blog.csdn.net/qq_24912401/article/details/90475290)

#### 图像矩阵在内存中的存储方式
**灰度图**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200222140059368.png)

> 灰度图一个像素由一个字节组成，即单通道

**RGB图**

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200222140119398.png)

> RGB图像的一个像素由以B，G，R顺序的三个字节组成，即三通道

#### 转化方式

> 1.使用图像容器Mat存储图像
> 2.使用容器函数，定位等方法遍历图像中的像素
> 3.将其保存在数组中

#### 常用函数

```cpp
Mat img;               //创建Mat容器
imread(path);          //传入图片
//       显示图片
/*   
namedWindow("图片名称", WINDOW_NORMAL);
imshow("图片名称", img);
waitKey(0);
*/
img.ptr<uchar>(i);            //ptr为指向图片的行指针，参数i为行数
imwrite("目标地址\\save.jpg", img);    //导出图片
```
      
#### 关键步骤
> 可以将RGB图像的宽乘3，使用字节数单位uchar，而不用其像素单位vec3b，与灰度图的单位统一，即可以统一处理

#### 注意

> 环境： vc15, DEBUG, X64
> Opencv410Debugx64属性配置文件要依据个人opencv位置修改
> test文件夹中方法为直接读取bmp文件，还需更新整理

#### 更新

>2020/2/22    首次上传
