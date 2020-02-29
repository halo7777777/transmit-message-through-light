#include "Decode.h"
void Decode::rotate(Mat& srcImg, Mat& dst)//传入源图像，目标矩阵,得到纠偏后的图像存在dst
{
	Mat newImg = srcImg;
	QRCodeDetector qrDetector;
	vector<Point2f> list;
	qrDetector.detect(srcImg, list);
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

unsigned char* Decode::decode(Mat& srcImg,int type,int& length)
{
	Mat dst;
	rotate(srcImg, dst);
	if (type == SINGLE || type == END)
	{
		length = getLength(dst);
	}
	else
	{
		length = MAXSIZE;
	}
	unsigned char* tmp = new unsigned char[length];
	for (int i = 0; i < length; i++) { tmp[i] = 0; }
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
			if(index>=length) return tmp;
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
			if (index >= length) return tmp;
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
			if (index >= length) return tmp;
			tmp[index++] = (unsigned char)code;
		}
	}

	return tmp;
}