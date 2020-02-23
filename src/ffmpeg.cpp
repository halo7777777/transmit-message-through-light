#include<iostream>
#include<windows.h>
#include<string>
#include<stdio.h>
#include"ffmpeg.h"

using namespace std;

	char fp_out1[100];//图片转视频函数中输出视频文件的名称
	char fp_in2[100];//视频转图片函数中输入视频文件的名称

	void picture_ts_video(int pic_num_persec)//将图片转化为视频，参数：视频帧数
	{
		string str1, str2, str3;
		char str4[5], str6[200];
		char a[50], b[50];
		sprintf_s(str4, "%d", pic_num_persec);
		cin >> fp_out1;
		str1 = "ffmpeg -f image2 -r ";
		str2 = " -i %d.jpg -vcodec mpeg4 ";
		str3 = str1 + str4 + str2 + fp_out1; //组合生成ffmpeg命令
		sprintf_s(str6, "%s", str3.c_str()); //转化类型
		system(str6);
	}
	void video_ts_picture()//将视频转化为图片
	{
		string str1, str2, str3, str5;
		char str6[200];
		str1 = "ffmpeg -i ";
		str2 = " %d.png";
		cin >> fp_in2;
		str3 = str1 + fp_in2 + str2; //组合生成ffmpeg命令
		sprintf_s(str6, "%s", str3.c_str()); //转化类型
		system(str6);
	}
