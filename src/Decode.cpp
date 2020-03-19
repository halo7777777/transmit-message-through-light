#pragma once
#include "Decode.h"
#include"code.h"
#include"FileConvert.h"
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


bool Decode::QrRate(float rate)
{
	//大概比例 不能太严格
	return rate > 0.3 && rate < 1.9;
}

bool Decode::QrColorRateX(cv::Mat& image, int flag)
{
	int nr = image.rows / 2;
	int nc = image.cols * image.channels();

	vector<int> vValueCount;
	vector<uchar> vColor;
	int count = 0;
	uchar lastColor = 0;

	uchar* data = image.ptr<uchar>(nr);
	for (int i = 0; i < nc; i++)
	{
		vColor.push_back(data[i]);
		uchar color = data[i];
		if (color > 0)
			color = 255;

		if (i == 0)
		{
			lastColor = color;
			count++;
		}
		else
		{
			if (lastColor != color)
			{
				vValueCount.push_back(count);
				count = 0;
			}
			count++;
			lastColor = color;
		}
	}

	if (count != 0)
		vValueCount.push_back(count);

	if (vValueCount.size() < 5 || vValueCount.size() > 7)
		return false;

	//横向黑白比例1:1:3:1:1
	int index = -1;
	int maxCount = -1;
	for (int i = 0; i < vValueCount.size(); i++)
	{
		if (i == 0)
		{
			index = i;
			maxCount = vValueCount[i];
		}
		else
		{
			if (vValueCount[i] > maxCount)
			{
				index = i;
				maxCount = vValueCount[i];
			}
		}
	}

	//左边 右边 都有两个值，才行
	if (index < 2)
		return false;
	if ((vValueCount.size() - index) < 3)
		return false;

	//黑白比例1:1:3:1:1
	float rate = ((float)maxCount) / 3.00;

	if (!QrRate(vValueCount[index - 2] / rate))
		return false;
	if (!QrRate(vValueCount[index - 1] / rate))
		return false;
	if (!QrRate(vValueCount[index + 1] / rate))
		return false;
	if (!QrRate(vValueCount[index + 2] / rate))
		return false;

	return true;
}

bool Decode::QrColorRateY(cv::Mat& image, int flag)
{
	int nc = image.cols / 2;
	int nr = image.rows;

	vector<int> vValueCount;
	int count = 0;
	uchar lastColor = 0;

	for (int i = 0; i < nr; i++)
	{
		uchar* data = image.ptr<uchar>(i, nc);
		uchar color;
		if (data[0] > 0 || data[1] > 0 || data[2] > 0)
			color = 255;
		else
			color = 0;

		if (i == 0)
		{
			lastColor = color;
			count++;
		}
		else
		{
			if (lastColor != color)
			{
				vValueCount.push_back(count);
				count = 0;
			}
			count++;
			lastColor = color;
		}
	}

	if (count != 0)
		vValueCount.push_back(count);

	if (vValueCount.size() < 5 || vValueCount.size() > 7)
		return false;

	//横向黑白比例1:1:3:1:1
	int index = -1;
	int maxCount = -1;
	for (int i = 0; i < vValueCount.size(); i++)
	{
		if (i == 0)
		{
			index = i;
			maxCount = vValueCount[i];
		}
		else
		{
			if (vValueCount[i] > maxCount)
			{
				index = i;
				maxCount = vValueCount[i];
			}
		}
	}

	//左边 右边 都有两个值，才行
	if (index < 2)
		return false;
	if ((vValueCount.size() - index) < 3)
		return false;

	//黑白比例1:1:3:1:1
	float rate = ((float)maxCount) / 3.00;

	if (!QrRate(vValueCount[index - 2] / rate))
		return false;
	if (!QrRate(vValueCount[index - 1] / rate))
		return false;
	if (!QrRate(vValueCount[index + 1] / rate))
		return false;
	if (!QrRate(vValueCount[index + 2] / rate))
		return false;
	return true;
}

bool Decode::IsQrColorRate(cv::Mat& image, int flag)
{
	if (!QrColorRateX(image, flag))
		return false;
	return QrColorRateY(image, flag);;
}

Mat Decode::CropImage(Mat& img, RotatedRect& rotatedRect)
{
	Point2f points[4];
	rotatedRect.points(points);
	int topLeftIndex = 0;
	double topLeftR = points[0].x * points[0].x + points[0].y * points[0].y;
	for (int i = 1; i < 4; i++) {
		double r = points[i].x * points[i].x + points[i].y * points[i].y;
		if (r < topLeftR) {
			topLeftIndex = i;
			topLeftR = r;
		}
	}
	double x1 = points[(topLeftIndex + 1) % 4].x - points[topLeftIndex].x, y1 = points[(topLeftIndex + 1) % 4].y - points[topLeftIndex].y;
	double x2 = points[(topLeftIndex + 3) % 4].x - points[topLeftIndex].x, y2 = points[(topLeftIndex + 3) % 4].y - points[topLeftIndex].y;
	double vX1 = x1, vY1 = y1, vX2 = x2, vY2 = y2;
	int width = (int)sqrt(vX1 * vX1 + vY1 * vY1), height = (int)sqrt(vX2 * vX2 + vY2 * vY2);
	if (img.dims == 3) {
		Mat ret(Size(width, height), CV_8UC3);
		for (int j = 0; j < ret.rows; j++) {
			for (int i = 0; i < ret.cols; i++) {
				double kx = (double)i / width, ky = (double)j / height;
				int x = (int)(points[topLeftIndex].x + kx * vX1 + ky * vX2), y = (int)(points[topLeftIndex].y + kx * vY1 + ky * vY2);
				if (x < 0)x = 0;
				else if (x >= img.cols)x = img.cols - 1;
				if (y < 0)y = 0;
				else if (y >= img.rows)y = img.rows - 1;
				ret.at<Vec3b>(j, i) = img.at<Vec3b>(y, x);
			}
		}
		return ret;
	}
	else {
		Mat ret(Size(width, height), CV_8UC1);
		for (int j = 0; j < ret.rows; j++) {
			for (int i = 0; i < ret.cols; i++) {
				double kx = (double)i / width, ky = (double)j / height;
				int x = (int)(points[topLeftIndex].x + kx * vX1 + ky * vX2), y = (int)(points[topLeftIndex].y + kx * vY1 + ky * vY2);
				if (x < 0)x = 0;
				else if (x >= img.cols)x = img.cols - 1;
				if (y < 0)y = 0;
				else if (y >= img.rows)y = img.rows - 1;
				ret.at<uchar>(j, i) = img.at<uchar>(y, x);
			}
		}
		return ret;
	}
}

bool Decode::QrPoint(vector<Point>& contour, Mat& img, int i)
{
	//最小大小限定
	RotatedRect rotated_rect = minAreaRect(contour);
	if (rotated_rect.size.height < 40 || rotated_rect.size.width < 40)
		return false;
	//将二维码从整个图上抠出来
	cv::Mat cropImg = CropImage(img, rotated_rect);
	int flag = i++;
	//横向黑白比例1:1:3:1:1
	bool result = IsQrColorRate(cropImg, flag);
	return result;
}

Mat Decode::transformCorner(Mat src, RotatedRect rect)
{
	Point center = rect.center;
	Point TopLeft = Point(cvRound(center.x), cvRound(center.y)) - Point(rect.size.height / 2, rect.size.width / 2);
	TopLeft.x = TopLeft.x > src.cols ? src.cols : TopLeft.x;
	TopLeft.x = TopLeft.x < 0 ? 0 : TopLeft.x;
	TopLeft.y = TopLeft.y > src.rows ? src.rows : TopLeft.y;
	TopLeft.y = TopLeft.y < 0 ? 0 : TopLeft.y;

	Rect RoiRect = Rect(TopLeft.x, TopLeft.y, rect.size.width, rect.size.height);
	double angle = rect.angle;
	Mat mask, roi, dst;
	Mat image;
	Size sz = src.size();
	mask = Mat::zeros(src.size(), CV_8U);
	vector<Point> contour;
	Point2f points[4];
	rect.points(points);
	for (int i = 0; i < 4; i++)
		contour.push_back(points[i]);
	vector<vector<Point>> contours;
	contours.push_back(contour);
	drawContours(mask, contours, 0, Scalar(1), -1);

	src.copyTo(dst, mask);

	Mat M = getRotationMatrix2D(center, angle, 1);
	warpAffine(dst, image, M, sz);
	roi = image(RoiRect);

	return roi;
}

double Decode::Ratete(Mat count)
{
	int number = 0;
	int allpixel = 0;
	for (int row = 0; row < count.rows; row++)
	{
		for (int col = 0; col < count.cols; col++)
		{
			if (count.at<uchar>(row, col) == 255)
			{
				number++;
			}
			allpixel++;
		}
	}
	return (double)number / allpixel;
}

bool Decode::isCorner(Mat& image)
{
	Mat mask, dstGopy;
	Mat dstGray;
	mask = image.clone();
	cvtColor(image, dstGray, COLOR_BGR2GRAY);
	threshold(dstGray, dstGray, 100, 255, THRESH_BINARY_INV);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(dstGray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++)
	{
		if (hierarchy[i][2] == -1 && hierarchy[i][3])
		{
			Rect rect = boundingRect(Mat(contours[i]));
			rectangle(image, rect, Scalar(0, 0, 255), 2);
			/******************由图可知最里面的矩形宽度占总宽的3/7***********************/
			if (rect.width < mask.cols * 2 / 7)      //2/7是为了防止一些微小的仿射
				continue;
			if (Ratete(dstGray(rect)) > 0.75)
			{
				rectangle(mask, rect, Scalar(0, 0, 255), 2);
				return true;
			}
		}
	}
	return  false;
}

void Decode::anchorSequence(vector<Point2f>& anchor_center)
{
	Point2f tmp_center[4], tmp;
	for (int i = 0; i < 4; i++)
		tmp_center[i] = anchor_center[i];
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 3; i++)
		{
			if ((tmp_center[i].x + tmp_center[i].y) > (tmp_center[i + 1].x + tmp_center[i + 1].y))
			{
				tmp = tmp_center[i]; tmp_center[i] = tmp_center[i + 1]; tmp_center[i + 1] = tmp;
			}
		}
	if (tmp_center[1].y < tmp_center[2].y) {
		tmp = tmp_center[1]; tmp_center[1] = tmp_center[2]; tmp_center[2] = tmp;
	}
	for (int i = 0; i < 4; i++)
		anchor_center.pop_back();
	anchor_center.push_back(tmp_center[1]);
	anchor_center.push_back(tmp_center[0]);
	anchor_center.push_back(tmp_center[2]);
	anchor_center.push_back(tmp_center[3]);
}

int Decode::findQranchor(Mat& srcImg, Mat& dst)
{
	if (!srcImg.data)
		return -1;
	Mat srcGray;
	int cols = srcImg.cols;
	int rows = srcImg.rows;
	vector<vector<Point>> qrPoint;
	vector<vector<Point>> contour2;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	cvtColor(srcImg, srcGray, COLOR_BGR2GRAY);
	threshold(srcGray, srcGray, 188, 255, THRESH_BINARY | THRESH_OTSU);
	Mat otsu_gray;
	//threshold(srcGray, otsu_gray, 150, 255, THRESH_BINARY);
	//threshold(srcGray, srcGray, 188, 255, THRESH_BINARY);

	findContours(srcGray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	int ic = 0;
	int parentIdx = -1;

	for (int i = 0; i < contours.size(); i++)
	{
		int k = i;
		int ic = 0;
		while (hierarchy[k][2] != -1)
		{
			if (ic == 0)
				parentIdx = i;
			k = hierarchy[k][2];
			ic++;
		}
		//有两个子轮廓才是二维码的顶点
		if (ic >= 2)
		{
			bool isQr = QrPoint(contours[parentIdx], srcGray, parentIdx);
			//保存找到的四个黑色定位角
			if (isQr)
				contour2.push_back(contours[parentIdx]);
			parentIdx = -1;
		}
	}

	vector<Point2f> qr_center, src_center;
	for (int i = 0; i < contour2.size(); i++)
	{

		double area = contourArea(contour2[i]);
		if (area < 100)
			continue;
		RotatedRect rect = minAreaRect(Mat(contour2[i]));
		double w = rect.size.width;
		double h = rect.size.height;
		double rate = min(w, h) / max(w, h);
		if (rate > 0.85)
		{
			Mat image = transformCorner(srcImg, rect);
			char tmp_name[20];
			if (true)//isCorner(image))
			{
				Point2f points[4];
				rect.points(points);
				qr_center.push_back(rect.center);
				qrPoint.push_back(contour2[i]);
			}
		}

	}

	for (int i = 0; i < qrPoint.size(); i++)
	{
		RotatedRect rect = minAreaRect(qrPoint[i]);
		Point2f boxpoint[4];
		rect.points(boxpoint);
		qrPoint[i].clear();
		for (int j = 0; j < 4; j++)
		{
			qrPoint[i].push_back(boxpoint[j]);
		}
	}

	Mat output;
	if (qr_center.size() == 4)
	{
		anchorSequence(qr_center);
		for (int i = 0; i < qr_center.size(); ++i)
			src_center.push_back(qr_center[i]);
		for (int i = 0; i < src_center.size(); ++i)
			line(srcImg, src_center[i], src_center[(i + 1) % 4], Scalar(0, 255, 255), 5);
		vector<Point2f> origin_center;
		double c = 7;
		double r = 89;
		int cons = 96;
		origin_center.push_back(Point2f(cols * c / cons, rows * r / cons));
		origin_center.push_back(Point2f(cols * c / cons, rows * c / cons));
		origin_center.push_back(Point2f(cols * r / cons, rows * c / cons));
		origin_center.push_back(Point2f(cols * r / cons, rows * r / cons));


		Mat warp_mat = getPerspectiveTransform(src_center, origin_center);




		warpPerspective(srcGray, output, warp_mat, srcImg.size());
		resize(output, output, Size(96, 96));
		dst = output;
		return 1;
	}
	else
	{
		return 0;
	}
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
	for (int i = 0; i < 2; i++)
	{
		Vec3b pix = srcImg.at<Vec3b>(16, i);
		typecode += k * getBit(pix);
		k *= 2;
	}
	switch (typecode)
	{
	case 0: //00
		return BEGIN;
	case 2://01
		return NORMAL;
	case 1://10
		return END;
	case 3:
		return SINGLE;
	}//小端法字节
}

int Decode::getLength(Mat& srcImg)
{
	int length = 0;
	int k = 1;
	for (int i = 0; i < 12; i++)//第16行[3,15]用来存长度
	{
		Vec3b pix = srcImg.at<Vec3b>(16, 4 + i);
		length += k * getBit(pix);
		k *= 2;
	}
	return length;
}


unsigned char* Decode::decode(Mat& dst, int& length, int& type, vector<unsigned char> &valid)
{


	type = getType(dst);
	unsigned char* tmp = NULL;
	length = getLength(dst);
	int tmplen;

	if (type == SINGLE || type == END)

	{
		tmplen = length;
		tmp = new unsigned char[tmplen];

	}
	else if (type == BEGIN || type == NORMAL)
	{
		tmplen = MAXSIZE;
		tmp = new unsigned char[tmplen];
	}
	for (int i = 0; i < tmplen; i++) { tmp[i] = 0; }
	//block A
	int index = 0;//暂存数组的下标
	for (int i = 17; i < 79; i++)//遍历行
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
			if (index >= tmplen) return tmp;
			tmp[index++] = (unsigned char)code;
		}
	}
	int newCrcA = 0;
	int k = 1;
	for (int i = 0; i < 16; i++)
	{

		Vec3b pix = dst.at<Vec3b>(79, i);
		newCrcA += k * getBit(pix);
		k *= 2;
	}

	int oriCRCA = BuildCRC_16(tmp, index);
	int AEND = index;
	if (oriCRCA != newCrcA)
	{
		for (int i = 0; i < index; i++)
		{
			valid.push_back(0x00);
		}
	} 
	else
	{
		for (int i = 0; i < index; i++)
		{
			valid.push_back(0xff);
		}
	}

	for (int i = 0; i < 16; i++)//遍历行
	{
		for (int part = 0; part < 8; part++)//计算字节
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

	for (int i = 16; i < 96; i++)//遍历行
	{
		for (int part = 0; part < 8; part++)//计算字节
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


	for (int i = 16; i < 79; i++)//遍历行
	{//block5
		for (int part = 8; part < 10; part++)//计算字节
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
	int newCrcB = 0;
	k = 1;
	for (int i = 80; i < 96; i++)
	{

		Vec3b pix = dst.at<Vec3b>(79, i);
		newCrcB += k * getBit(pix);
		k *= 2;
	}
	//0,1是A区域，AEND = 2
	//2,3,4是B区域 LENGTH=3,INDEX = 5
	int len = index - AEND;
	int oriCRCB = BuildCRC_16(tmp+AEND, len);
	FileConvert converter;
	char name[] = "BLOCK.bin";
	converter.ByteToFile(tmp + AEND, name, len);


	if (oriCRCB != newCrcB)
	{
		for (int i = 0; i < len; i++)
		{
			valid.push_back(0x00);
		}
	}
	else
	{
		for (int i = 0; i < len; i++)
		{
			valid.push_back(0xff);
		}
	}


	return tmp;

}

int Decode::getFlag(Mat& srcImg)
{
	int flagCode = 0;//code=3
	int k = 1;//K为二进制运算的系数
	for (int i = 2; i < 4; i++)
	{
		Vec3b pix = srcImg.at<Vec3b>(16, i);
		flagCode += k * getBit(pix);
		k *= 2;
	}
	return flagCode;
}

double Decode::getRate(Mat& srcImg)
{
	int counter = 0;
	for (int i = 0; i < 96; i++)
	{
		for (int j = 0; j < 96; j++)
		{
			Vec3b pix = srcImg.at<Vec3b>(i, j);
			int p = getBit(pix);
			if (p == 1) counter++;
		}
	}
	double rate = (1.0 * counter) / (96 * 96);
	return rate;
}



int Decode::BuildCRC_16(unsigned char* info, int len)         //CRC-16/CCITT        x16+x12+x5+1 
{
	unsigned int crc_ta_8[256] =
	{ /* CRC 字节余式表 */
		0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
		0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
		0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
		0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
		0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
		0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
		0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
		0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
		0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
		0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
		0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
		0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
		0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
		0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
		0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
		0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
		0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
		0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
		0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
		0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
		0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
		0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
		0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
		0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
		0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
		0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
		0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
		0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
		0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
		0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
		0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
		0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
	};
	uint16_t crc = 0;
	//得到crc
	while (len-- != 0)
	{
		unsigned int high = (unsigned int)(crc / 256); //取CRC高8位
		crc <<= 8;
		crc ^= crc_ta_8[high ^ *info];
		info++;
	}
	return crc;
}