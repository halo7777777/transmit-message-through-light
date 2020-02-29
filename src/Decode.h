#pragma once
#include<opencv2/opencv.hpp>
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


public:
	Decode(int size=96) :ROW(size), COL(size){}
	unsigned char* decode(Mat& srcImg, int type, int& length);
	int getType(Mat& srcImg);
	int getLength(Mat& srcImg);
	void rotate(Mat& srcImg, Mat& dst);
};

