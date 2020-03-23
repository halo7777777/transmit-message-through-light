## Transmit-message-through-light
----
#### 项目介绍：
> 本项目是一个基于c++开发的项目,是以光作为传输介质(物理层)来模拟数据的传输的计算机网络项目

----
#### 使用方法：

>在bin目录下有可执行文件Decode.exe与Encode.exe

- **编码Encode**

> 运行Encode.exe
>得到下面的程序运行界面
>![error](https://pic.downk.cc/item/5e78d2c09dbe9d88c56c5597.png)

> 输入 encode [二进制文件名] [生成的视频名] [视频长度(单位ms)]
></br> **Example:** 
></br>  encode 1.bin 1.mp4 1000

> 等待运行完毕即可得到生成的视频文件

</br>

- **手机拍摄生成的视频并传入电脑**

</br>

- **解码Decode：**
> 运行Decode.exe
>得到下面的程序运行界面
![error](https://pic.downk.cc/item/5e78d2cf9dbe9d88c56c6157.png)

> 输入 encode [手机拍摄的视频名] [生成的二进制文件名] [二进制校验文件名]
></br> **Example:**
></br> decode 1.mp4 1.bin test.bin

> 等待运行完毕即可得到生成的二进制文件

-----
#### 项目性能
传输速率(bps) | 误码率(%) | 测评时间
---|---|---
73584 | 0.00|2020-3-14 20：00
 | |
  | |
   | |

