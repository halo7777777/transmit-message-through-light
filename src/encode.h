#pragma once
#pragma once
#include<opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include<cstdio>

namespace Code
{
	using namespace cv;
	using namespace std;
	enum class FrameType;

	void BulidQrPoint(Mat& mat);              //二维码定位码
	void BulidInfoRect(Mat& mat, unsigned char* info, unsigned long len, int areaID);
	void BulidFrameFlag(Mat& mat, FrameType frameType, unsigned long tailLen);
	void BulidPicNum(Mat& mat, int PicNum);
	Mat CodeFrame(FrameType frameType, unsigned char* info, unsigned long tailLen, int PicNum);
	Mat amplify(const Mat& src, int tag);
	void Main(unsigned char* info, unsigned long len, const char* savePath, const char* outputFormat, int tag); // tag标记是否裁剪图，flag标记使用颜色
	Mat transform(const Mat& src);        //裁剪转换为96×96
	void BuildCRC_16(Mat& mat, unsigned char* info, int len, int area_No);             //using: CRC-16/CCITT        x16+x12+x5+1 
}