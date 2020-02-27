#include<iostream>
#include"Decode.h"
#include"FileConvert.h"
#include<cstdio>
#include<cstdlib>
#include"code.h"
using namespace std;
using namespace cv;
int main()
{
	int function;
	cout << "function one:encode file to video" << endl;
	cout << "function two:decode video to file" << endl;
	cout << "input function you choose" << endl;
	cin >> function;
	FileConvert converter;
	if (function == 1)
	{
		//选择输入字符串or生成随机比特流//根据字符串生成文件的接口还没有写
		unsigned long size;
		cout << "Please input size :";
		cin >> size;//需要生成文件的大小，单位字节
		char filename[] = "in.bin";
		converter.GenerateRandFile(filename, size);//生成随机二进制文件
		unsigned char* bytes = converter.FileToByte(filename, &size);//读取文件生成的Byte数组
		/*
		一段由bytes生成图像的代码
		命名格式为%d.png
		输出在imageSrc目录

		*/
		int fps = 10;//测试用每秒10帧试试
		converter.PicTransToVideo(fps);
	}	/*

	用手机录制视频并拍摄
	*/
	else if (function == 2)
	{
		int indexptr = 0;//用于存储目的数组的当前下标
		Decode decoder;
		int length;
		converter.VideoTransToPic();
		unsigned char* output;
		int numOfPic=0;//获取imageOutput目录下文件个数
		Mat img;
		img = imread("imageOutput\\0.png");//第0张图片单独处理
		int type = decoder.getType(img);
		if (type == SINGLE)//如果只有单张图
		{
			output = decoder.decode(img, SINGLE, length);
		}
		else if (type == BEGIN)//多张图片的第一张图片
		{
			length = decoder.getLength(img);
			output = new unsigned char[length];//用于储存数据的数组
		}//不可能是end和normal了


		for (int i = 1; i < numOfPic; i++)
		{
			string path = "imageOutput\\";
			char filename[10];
			sprintf_s(filename, "%d.png", i);
			img = imread(path + filename);
			type = decoder.getType(img);
			int tmplen;
			unsigned char* tmp = decoder.decode(img, type, tmplen);//将当前二维码的数据暂存于数组中
			memcpy(output + indexptr, tmp, tmplen);//复制到最终的数组里
			indexptr += tmplen;
			delete[]tmp;
		}//遍历所有图片
		char filename[] = "out.bin";
		converter.ByteToFile(output, filename, length);

	}
}

