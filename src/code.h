#pragma once
#include<opencv2/opencv.hpp>

#include<cstdio>

namespace Code
{
	using namespace cv;
	using namespace std;
	enum class FrameType;
	void BulidSafeArea(Mat& mat);             //安全带
	void BulidQrPoint(Mat& mat);              //定位图案
	void BulidInfoRect(Mat& mat, const char* info, int len, int areaID);
	void BulidFrameFlag(Mat& mat, FrameType frameType, int tailLen);
	Mat CodeFrame(FrameType frameType, const char* info, int tailLen);
	Mat amplify(const Mat& src);
	void Main(const char* info, int len, const char* savePath, const char* outputFormat);  //传入文件地址，长度，保存路径，保存格式
}