#include<vector>
#include <opencv2/opencv.hpp>
#include<algorithm>
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/core/core.hpp"
#include<vector>
#include<opencv2/imgproc/types_c.h>
#include"Decode.h"
using namespace std;
using namespace cv;
const int COL = 96;
const int ROW = 96;
void showBit(Vec3b pix)
{
	int a, b, c;
	a = pix[0];
	b = pix[1];
	c = pix[2];
	cout << a << " " << b << " " << c << endl;
}
bool cmp(vector<Point> a, vector<Point> b)
{
	return contourArea(a) > contourArea(b);
}

void rotate(Mat& srcImg, Mat& dst);

int main()
{
	Mat img = imread("E:\\vsproject\\getts.png");
	//imshow("name", img);
	if (img.empty())
	{
		printf("reading image file fails \n");
		return 0;
	}
	Decode decoder;
	int len;
	unsigned char* s = decoder.decode(img, SINGLE, len);	cout << len;
	for (int i = 0; i < len; i++)
	{
		printf_s("%c", s[i]);
	}
	//resize(img, dst, Size(100, 100));
	//imshow("win", dst);
	//waitKey(0);
	//GetCropCode(img, dst);
	//imshow("win", dst);
	//waitKey(0);
	//imshow("name", dst);
	//int valid_length = 0;
	//int j = 1;
	/**for (int i = 0; i < 8; i++)
	{
		Vec3b pix= dst.at<Vec3b>(19,2+i);
		//showBit(pix);
		cout << getBit(pix) << " ";
		//valid_length += j * getBit(pix);
		//j *= 2;
	}
	//cout << valid_length << endl;
	//int charsum = 0;
	//block a
	/*for (int i = 17; i < 80; i++)
	{
		for (int part = 0; part < 2; part++)
		{
			int code=0;
			int index = 1;
			for (int j = 0; j < 8; j++)
			{
				Vec3b pix = dst.at<Vec3b>(i, j+part*8);
				code += index * getBit(pix);
				index *= 2;
			}
			charsum++;
			char c = (char)code;
			printf_s("%c",code);
			if (charsum > 10) return 0;
			//cout << code << " ";
		}
	}
	/*int code = 0;
	j = 1;
	for (int i = 0; i < 8; i++)
	{
		Vec3b pix = dst.at<Vec3b>(19, 3+i);
		//showBit(pix);
		//cout << getBit(pix) << " ";
		code += j * getBit(pix);
		j *= 2;
	}
	cout << code;
	cout << endl << charsum << endl;
	/*QRCodeDetector QRdetecter;
	vector<Point> list;
	Mat  res;

	QRdetecter.detect(img, list);

	/*for (int i = 0; i < list.size(); i++)
	{
		if (i == 3)
			line(img, list[i], list[0], Scalar(0, 255, 0), 2);
		else
			line(img, list[i], list[i + 1], Scalar(0, 255, 0), 2);
	}
//	//cout << list[0];
//	//int maxwidth = img.cols;
//	//int maxheight = img.rows;
//	//int width = max(list[1].x - list[0].x, list[2].x - list[3].x);
//	//int height= max(list[2].y - list[1].y, list[3].y - list[0].x);
//	//if (list[0].x + width > maxwidth) width = maxwidth - list[0].x;
//	//if (list[0].y + height > maxheight) height = maxheight - list[0].y;
//	
//
	Rect rect =boundingRect(list);
//
	res = img(rect);
//	
	imshow("ret", res);
	waitKey(0);*/
//
//
	return 0;
}

/*void detect(Mat src)
{
	Mat img_gray;
	vector<Point> list;
	QRCodeDetector QRdetecter;
	QRdetecter.detect(src, list);
	if (list.empty())
	{
		cout << "found nothing" << endl;
		return;
	}
	for (int i = 0; i < list.size(); i++)
	{
		if (i == 3)
			line(src, list[i], list[0], Scalar(0, 255, 0), 2);
		else
			line(src, list[i], list[i + 1], Scalar(0, 255, 0), 2);
	}
	//Rect rect = boundingRect(list);
	//Mat img_src = src(rect);
	//cvtColor(src, img_gray, CV_BGR2GRAY);//将图片转换为灰度图



	//Mat edge;
	//Canny(img_gray, edge, 100, 200);
	//imshow("test", edge);
	//waitKey(0);

	//vector<vector<Point>> contours;
	//vector<Vec4i> hierarchy;
	//findContours(edge, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	//imshow("test", edge);
	//waitKey(0);
	//vector<vector<Point>> foundcontours;
	//for (int i = 0; i < contours.size(); i++)
	//{
	//	int k = i;
	//	int c = 0;
	//	while (hierarchy[k][2] != -1)
	//	{
	//		k = hierarchy[k][2];
	//		c++;
	//	}
	//	if (c >=3)
	//	{
	//		foundcontours.push_back(contours[i]);
	//	}

	//}
	//

	//Mat Contours = Mat::zeros(src.size(), CV_8UC1);
	////drawContours(src, foundcontours, -1, Scalar(255), 3);
	//cout << foundcontours.size() << endl;
	//sort(foundcontours.begin(), foundcontours.end(), cmp);
	//Mat  res;
	////Rect rect = boundingRect(foundcontours[0]);
	//cout << foundcontours[0];
	//drawContours(Contours, foundcontours, 0, Scalar(255), 3);
	////imwrite("output.png", src);
	////cout << rect;
	imshow("test", src);
	waitKey(0);
}*/

void rotate(Mat& srcImg, Mat& dst)
{
	Mat newImg = srcImg;
	QRCodeDetector qrDetector;
	vector<Point2f> list;
	qrDetector.detect(newImg, list);
	if (list.empty())
	{
		cout << "false";
		return;
	}
	Mat warpPerspective_mat(3, 3, CV_32FC1);//3，3旋转矩阵
	Mat warpPerspective_dst = Mat::zeros(256, 256, newImg.type());//储存旋转后的目标
	vector<Point2f> dstTri;//三角形
	dstTri.push_back(Point2f(0, 0));
	dstTri.push_back(Point2f(255, 0));
	dstTri.push_back(Point2f(255, 255));
	dstTri.push_back(Point2f(0, 255));

	warpPerspective(newImg, warpPerspective_dst, warpPerspective_mat, warpPerspective_dst.size());
	imshow("rotate", newImg);
	waitKey(0);


	cvtColor(warpPerspective_dst, dst, COLOR_BGR2GRAY);
	threshold(dst, dst, 150, 255, THRESH_BINARY);
}