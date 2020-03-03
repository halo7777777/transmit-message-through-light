#include "Decode.h"

Point Decode::Center_cal(vector<vector<Point> > contours, int i)
{
	int centerx = 0, centery = 0, n = static_cast<int>(contours[i].size());
	//在提取的小正方形的边界上每隔周长个像素提取一个点的坐标，
	//求所提取四个点的平均坐标（即为小正方形的大致中心）
	centerx = (contours[i][n / 4].x + contours[i][n * 2 / 4].x + contours[i][3 * n / 4].x + contours[i][n - 1].x) / 4;
	centery = (contours[i][n / 4].y + contours[i][n * 2 / 4].y + contours[i][3 * n / 4].y + contours[i][n - 1].y) / 4;
	Point point1 = Point(centerx, centery);
	return point1;
}

void Decode::locate(Mat& src,Mat& dst)
{
	Mat src_all = src.clone();
	RNG rng(12345);
	Mat src_gray;
	cvtColor(src, src_gray, CV_BGR2GRAY);

	vector<vector<Point> > contours, contours2;
	vector<Vec4i> hierarchy;
	Mat drawing = Mat::zeros(src.size(), CV_8UC3);
	Mat drawing2 = Mat::zeros(src.size(), CV_8UC3);
	Mat drawingAllContours = Mat::zeros(src.size(), CV_8UC3);
	Scalar color = Scalar(1, 1, 255);

	Canny(src_gray, drawingAllContours, 100, 255);
	//接下来开始找定位角

	findContours(drawingAllContours, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		int k = i;
		int c = 0;
		while (hierarchy[k][2] != -1)
		{
			k = hierarchy[k][2];
			c++;
			if (c >= 5)
			{
				contours2.push_back(contours[i]);
				drawContours(drawing, contours, i,CV_RGB(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 1, 8);
			}
		}
	}

	for (int i = 0; i < contours2.size(); i++)
		drawContours(drawing2, contours2, i,CV_RGB(rng.uniform(100, 255), rng.uniform(100, 255), rng.uniform(100, 255)), -1, 4,hierarchy[0][2], 0, Point());
	Point point[3];
	for (int i = 0; i < contours2.size(); i++) 
	{
		point[i] = Center_cal(contours2, i);
	}
	int area = 0;
	area = static_cast<int>(contourArea(contours2[1]));
	int area_side = cvRound(sqrt(double(area)));
	for (int i = 0; i < contours2.size(); i++)
	{
		//画出三个定位角的中心连线
		line(drawing2, point[i % contours2.size()], point[(i + 1) % contours2.size()], color, area_side / 2, 8);
	}
	//	接下来要框出这整个二维码
	Mat gray_all, threshold_output_all;
	vector<vector<Point> > contours_all;
	vector<Vec4i> hierarchy_all;
	cvtColor(drawing2, gray_all, CV_BGR2GRAY);
	threshold(gray_all, threshold_output_all, 45, 255, THRESH_BINARY);
	findContours(threshold_output_all, contours_all, hierarchy_all, RETR_EXTERNAL, CHAIN_APPROX_NONE,
		Point(0, 0));//RETR_EXTERNAL表示只寻找最外层轮廓
	Point2f fourPoint2f[4];
	//求最小包围矩形
	RotatedRect rectPoint = minAreaRect(contours_all[0]);

	Rect myRect = boundingRect(contours_all[0]);

	//将rectPoint变量中存储的坐标值放到 fourPoint的数组中
	rectPoint.points(fourPoint2f);

	Mat resultImage = Mat(src_all, myRect);
	Mat resizeImage;
	resize(resultImage, resizeImage, Size(96, 96));

	cvtColor(resizeImage, dst, COLOR_BGR2GRAY);
	threshold(dst, dst, 150, 255, THRESH_BINARY | THRESH_OTSU);
	cvtColor(dst, dst, COLOR_GRAY2BGR);//颜色恢复
	//imshow("test", dst);
	//waitKey(0);
}

void Decode::rotate(Mat& srcImg, Mat& dst)//传入源图像，目标矩阵,得到纠偏后的图像存在dst
{
	Mat newImg = srcImg;
	QRCodeDetector qrDetector;
	vector<Point2f> list;
	qrDetector.detect(srcImg, list);
	if (list.empty())
	{
		cout << "noting to find";
		return;
	}
	Mat warpPerspective_mat(3, 3, CV_32FC1);//3，3旋转矩阵
	Mat warpPerspective_dst = Mat::zeros(ROW, COL, newImg.type());//旋转后的目标

	vector<Point2f> dstRect;//目的点
	dstRect.push_back(Point2f(0, 0));
	dstRect.push_back(Point2f(ROW-1, 0));
	dstRect.push_back(Point2f(ROW-1, COL-1));
	dstRect.push_back(Point2f(0, COL-1));
	warpPerspective_mat = getPerspectiveTransform(list, dstRect);//生成旋转矩阵
	warpPerspective(newImg, warpPerspective_dst, warpPerspective_mat, warpPerspective_dst.size());//进行透视变换

	dst = warpPerspective_dst;
	cvtColor(warpPerspective_dst, dst, COLOR_BGR2GRAY);
	threshold(dst, dst, 150, 255, THRESH_BINARY | THRESH_OTSU);
	cvtColor(dst, dst, COLOR_GRAY2BGR);//颜色恢复
	//srcImg = dst;
}

int Decode::getBit(Vec3b pix)
{
	int a, b, c;
	a = pix[0];
	b = pix[1];
	c = pix[2];
	if (a == 0 && b == 0 && c == 0)
	{
		return 0;//黑
	}
	else if (a == 255 && b == 255 && c == 255)
	{
		return 1;//白
	}
}

int Decode::getType(Mat& srcImg)
{
	int typecode = 0;//code=3
	int k = 1;//K为二进制运算的系数
	for (int i = 0; i < 4; i++)
	{
		Vec3b pix = srcImg.at<Vec3b>(16, i);
		typecode += k * getBit(pix);
		k *= 2;
	}
	switch (typecode)
	{
	case 3: //1100
		return BEGIN;
	case 0://0000
		return NORMAL;
	case 12://0011
		return END;
	case 15:
		return SINGLE;
	}//小端法字节
}

int Decode::getLength(Mat& srcImg)
{
	int length = 0;
	int k = 1;
	for (int i = 0; i < 12; i++)
	{
		Vec3b pix = srcImg.at<Vec3b>(16, 4+i);
		length += k * getBit(pix);
		k *= 2;
	}
	return length;
}

unsigned char* Decode::decode(Mat& srcImg,int& length,int& type)
{
	Mat dst;
	locate(srcImg, dst);


	type = getType(dst);
	unsigned char* tmp=NULL;
	length = getLength(dst);
	int tmplen;
	if (type == SINGLE || type == END||type==NORMAL)
	{
		tmplen = length;
		tmp = new unsigned char[tmplen];

	}
	else if (type == BEGIN)
	{
		tmplen = MAXSIZE;
		 tmp = new unsigned char[tmplen];
	}
	for (int i = 0; i < tmplen; i++) { tmp[i] = 0; }
	//block A
	int index = 0;//暂存数组的下标
	for (int i = 17; i < 80; i++)//遍历行
	{
		for (int part = 0; part < 2; part++)//计算字节
		{
			int code = 0;
			int k = 1;
			for (int j = 0; j < 8; j++)
			{
				Vec3b pix = dst.at<Vec3b>(i, j + part * 8);
				code += k * getBit(pix);
				k *= 2;
			}
			if(index>= tmplen) return tmp;
			tmp[index++] = (unsigned char)code;
		}
	}
	//block A解码完成

	//Block B解码

	for (int i = 0; i < 16; i++)//遍历行
	{
		for (int part = 0; part < 8; part++)//计算字节
		{
			int code = 0;
			int k = 1;
			for (int j = 0; j < 8; j++)
			{
				Vec3b pix = dst.at<Vec3b>(i, 16+j + part * 8);
				code += k * getBit(pix);
				k *= 2;
			}
			if (index >= tmplen) return tmp;
			tmp[index++] = (unsigned char)code;
		}
	}

	//Block B解码完成

	//Block C解码开始

	for (int i = 16; i < 96; i++)//遍历行
	{
		for (int part = 0; part < 10; part++)//计算字节
		{
			int code = 0;
			int k = 1;
			for (int j = 0; j < 8; j++)
			{
				Vec3b pix = dst.at<Vec3b>(i, 16 + j + part * 8);
				code += k * getBit(pix);
				k *= 2;
			}
			if (index >= tmplen) return tmp;
			tmp[index++] = (unsigned char)code;
		}
	}

	return tmp;

}