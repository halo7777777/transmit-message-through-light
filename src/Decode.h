#pragma once
#include<opencv2/opencv.hpp>

#include "opencv2/imgproc.hpp"

#include<opencv2/imgproc/types_c.h>
#define BEGIN 0
#define NORMAL 1
#define END 2
#define SINGLE 3
#define MAXSIZE 1054

using namespace std;
using namespace cv;
class Decode
{
private:
	const int ROW;
	const int COL;
	int  getBit(Vec3b pix);
	int getType(Mat& srcImg);
	void rotate(Mat& srcImg, Mat& dst);
	int getLength(Mat& srcImg);
	Point Center_cal(vector<vector<Point> > contours, int i);
	void locate(Mat& src, Mat& dst);

public:
	Decode(int size=96) :ROW(size), COL(size){}
	unsigned char* decode(Mat& srcImg, int& length, int& type);

};
