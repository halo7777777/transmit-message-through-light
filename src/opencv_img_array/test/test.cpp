#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<iomanip>
#include <opencv2/opencv.hpp>
//#include "opencv2/highgui/highgui.hpp" 
//#include "opencv2/core/core.hpp"
#include"read_save.h"

using namespace std;
using namespace cv;

unsigned int** out_r;
unsigned int** out_g;
unsigned int** out_b;

void decode()      //解码
{

	char readPath[] = "C:\\Users\\18705996097\\Desktop\\1200px-ISO_C++_Logo.svg.bmp";
	readBmp(readPath);
	// 输出整体图像信息
	cout << "\nwidth=" << bmpWidth << "\nheight=" << bmpHeight << "\nbiBitCount=" << biBitCount << endl;
	// 图像的字节数
	int linebyte1 = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
	int n = 0, m = 0, count_pixel = 0;

	out_r = new unsigned int* [bmpHeight];        //开辟指针数组
	for (int i = 0; i < bmpHeight; i++)
		out_r[i] = new unsigned int[bmpWidth];

	out_g = new unsigned int* [bmpHeight];        //开辟指针数组
	for (int i = 0; i < bmpHeight; i++)
		out_g[i] = new unsigned int[bmpWidth];

	out_b = new unsigned int* [bmpHeight];        //开辟指针数组
	for (int i = 0; i < bmpHeight; i++)
		out_b[i] = new unsigned int[bmpWidth];

	//初始化原始像素的数组。

	if (biBitCount == 8)          //256色图像一个像素3个字节
	{
		Mat img_8 = Mat(bmpHeight, bmpWidth, CV_8UC1);//图像img2：row*col大小
		for (int i = 0; i < bmpHeight; i++)
		{
			for (int j = 0; j < bmpWidth; j++)
				img_8.at<uchar>(bmpHeight - 1 - i, j) = pBmpBuf[j+bmpWidth * i];
		}
		namedWindow("new");
		imshow("new", img_8);
		waitKey(0);
	}
	else if (biBitCount == 24)         //24位图像一个像素3个字节
	{
		for (int i = 0; i < bmpHeight; i++)
		{
			for (int j = 0; j < bmpWidth; j++)
			{
				out_b[bmpHeight - 1 - i][j] = pBmpBuf[j * 3 + bmpWidth * i * 3];             //倒着存入,位图数据在文件中的排列顺序是从左下角到右上角
				out_g[bmpHeight - 1 - i][j] = pBmpBuf[j * 3 + 1 + bmpWidth * i * 3];
				out_r[bmpHeight - 1 - i][j] = pBmpBuf[j * 3 + 2 + bmpWidth * i * 3];
			}
		}
		Mat img_24(bmpHeight, bmpWidth, CV_8UC3);
		for (int i = 0; i < bmpHeight; i++) 
		{
			for (int j = 0; j < bmpWidth; j++) 
			{
				img_24.at<Vec3b>(i, j)[0] = out_b[i][j];           //依次按bgr存，i为y轴，j为x轴
				img_24.at<Vec3b>(i, j)[1] = out_g[i][j];
				img_24.at<Vec3b>(i, j)[2] = out_r[i][j];
			}
		}

		namedWindow("new");
		imshow("new", img_24);
		waitKey(0);
		//imwrite("D:\\C++_file\\image_deal_C++\\11.bmp", img_data);
	}

	//清除缓冲区，pBmpBuf和pColorTable是全局变量，在文件读入时申请的空间
	delete[]pBmpBuf;
	if (biBitCount == 8)
		delete[]pColorTable;

}

int main()
{
	decode();
	system("pause");
	return 0;
}
