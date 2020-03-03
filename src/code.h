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
	void BulidSafeArea(Mat& mat);             //安全带
	void BulidQrPoint(Mat& mat);              //定位图案
	void BulidInfoRect(Mat& mat, unsigned char* info, unsigned long len, int areaID);
	void BulidFrameFlag(Mat& mat, FrameType frameType, unsigned long tailLen);
	void BulidPicNum(Mat& mat, int PicNum);
	Mat CodeFrame(FrameType frameType, unsigned char* info, unsigned long tailLen, int PicNum);
	Mat amplify(const Mat& src);
	void Main(unsigned char* info, unsigned long len, const char* savePath, const char* outputFormat);
}