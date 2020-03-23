## Source Code:


> 源代码主要分成两个部分：
>  1.编码encode部分
>  2.解码decode部分
----

#### Encode
> - encode_main.cpp  
编码部分的主函数

> - encode.cpp
实现了将输入的数据流转化为类二维码图片

> - FileConvert.cpp
实现了 随机生成二进制文件
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 文件<->数据流
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;图片<->视频
等数据文件的相互转换

----

#### Decode
> - decode_main.cpp
解码部分的主函数

> - Decode.cpp
实现了类二维码图片的定位识别等功能，将一张图片转换为数据信息

> - Qrdecode.cpp
通过调用Decode.cpp，实现了批量处理多张二维码图片，将其转化为数据信息

> - FileConvert.cpp
实现了 随机生成二进制文件
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 文件<->数据流
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;图片<->视频
等数据文件的相互转换