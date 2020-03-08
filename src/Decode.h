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

	
	void rotate(Mat& srcImg, Mat& dst);//弃用
	
	Point Center_cal(vector<vector<Point> > contours, int i);//弃用
	void locate(Mat& src, Mat& dst);//弃用
	bool QrRate(float rate);
	bool QrColorRateX(cv::Mat& image, int flag);
	bool QrColorRateY(cv::Mat& image, int flag);
	bool IsQrColorRate(cv::Mat& image, int flag);
	Mat CropImage(Mat& img, RotatedRect& rotatedRect);
	bool QrPoint(vector<Point>& contour, Mat& img, int i);
	Mat transformCorner(Mat src, RotatedRect rect);
	double Ratete(Mat count);
	bool isCorner(Mat& image);
	void anchorSequence(vector<Point2f>& anchor_center);
	//
	


public:
	Decode(int size=96) :ROW(size), COL(size){}
	unsigned char* decode(Mat& srcImg, int& length, int& type);
	int findQranchor(Mat& srcImg, Mat& dst);
	int  getBit(Vec3b pix);//输入坐标点，获取 01 
	int getType(Mat& srcImg);//获取二维码类型 //传入图片需要96x96
	int getLength(Mat& srcImg);//传入图片需要96x96
};
