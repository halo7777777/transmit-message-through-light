#pragma once
#include "Decode.h"

Point Decode::Center_cal(vector<vector<Point> > contours, int i)
{
	int centerx = 0, centery = 0, n = static_cast<int>(contours[i].size());
	//����ȡ��С�����εı߽���ÿ���ܳ���������ȡһ��������꣬
	//������ȡ�ĸ����ƽ�����꣨��ΪС�����εĴ������ģ�
	centerx = (contours[i][n / 4].x + contours[i][n * 2 / 4].x + contours[i][3 * n / 4].x + contours[i][n - 1].x) / 4;
	centery = (contours[i][n / 4].y + contours[i][n * 2 / 4].y + contours[i][3 * n / 4].y + contours[i][n - 1].y) / 4;
	Point point1 = Point(centerx, centery);
	return point1;
}

void Decode::locate(Mat& src, Mat& dst)
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
	//��������ʼ�Ҷ�λ��

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
				drawContours(drawing, contours, i, CV_RGB(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 1, 8);
			}
		}
	}

	for (int i = 0; i < contours2.size(); i++)
		drawContours(drawing2, contours2, i, CV_RGB(rng.uniform(100, 255), rng.uniform(100, 255), rng.uniform(100, 255)), -1, 4, hierarchy[0][2], 0, Point());
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
		//����������λ�ǵ���������
		line(drawing2, point[i % contours2.size()], point[(i + 1) % contours2.size()], color, area_side / 2, 8);
	}
	//	������Ҫ�����������ά��
	Mat gray_all, threshold_output_all;
	vector<vector<Point> > contours_all;
	vector<Vec4i> hierarchy_all;
	cvtColor(drawing2, gray_all, CV_BGR2GRAY);
	threshold(gray_all, threshold_output_all, 45, 255, THRESH_BINARY);
	findContours(threshold_output_all, contours_all, hierarchy_all, RETR_EXTERNAL, CHAIN_APPROX_NONE,
		Point(0, 0));//RETR_EXTERNAL��ʾֻѰ�����������
	Point2f fourPoint2f[4];
	//����С��Χ����
	RotatedRect rectPoint = minAreaRect(contours_all[0]);

	Rect myRect = boundingRect(contours_all[0]);

	//��rectPoint�����д洢������ֵ�ŵ� fourPoint��������
	rectPoint.points(fourPoint2f);

	Mat resultImage = Mat(src_all, myRect);
	Mat resizeImage;
	resize(resultImage, resizeImage, Size(96, 96));

	cvtColor(resizeImage, dst, COLOR_BGR2GRAY);
	threshold(dst, dst, 150, 255, THRESH_BINARY | THRESH_OTSU);
	cvtColor(dst, dst, COLOR_GRAY2BGR);//��ɫ�ָ�
	//imshow("test", dst);
	//waitKey(0);
}

void Decode::rotate(Mat& srcImg, Mat& dst)//����Դͼ��Ŀ�����,�õ���ƫ���ͼ�����dst
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
	Mat warpPerspective_mat(3, 3, CV_32FC1);//3��3��ת����
	Mat warpPerspective_dst = Mat::zeros(ROW, COL, newImg.type());//��ת���Ŀ��

	vector<Point2f> dstRect;//Ŀ�ĵ�
	dstRect.push_back(Point2f(0, 0));
	dstRect.push_back(Point2f(ROW - 1, 0));
	dstRect.push_back(Point2f(ROW - 1, COL - 1));
	dstRect.push_back(Point2f(0, COL - 1));

	warpPerspective_mat = getPerspectiveTransform(list, dstRect);//������ת����
	warpPerspective(newImg, warpPerspective_dst, warpPerspective_mat, warpPerspective_dst.size());//����͸�ӱ任

	dst = warpPerspective_dst;
	cvtColor(warpPerspective_dst, dst, COLOR_BGR2GRAY);
	threshold(dst, dst, 150, 255, THRESH_BINARY | THRESH_OTSU);
	cvtColor(dst, dst, COLOR_GRAY2BGR);//��ɫ�ָ�
	//srcImg = dst;
}

bool Decode::QrRate(float rate)
{
	//��ű��� ����̫�ϸ�
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

	//����ڰױ���1:1:3:1:1
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

	//��� �ұ� ��������ֵ������
	if (index < 2)
		return false;
	if ((vValueCount.size() - index) < 3)
		return false;

	//�ڰױ���1:1:3:1:1
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

	//����ڰױ���1:1:3:1:1
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

	//��� �ұ� ��������ֵ������
	if (index < 2)
		return false;
	if ((vValueCount.size() - index) < 3)
		return false;

	//�ڰױ���1:1:3:1:1
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
	//��С��С�޶�
	RotatedRect rotated_rect = minAreaRect(contour);
	if (rotated_rect.size.height < 40 || rotated_rect.size.width < 40)
		return false;
	//����ά�������ͼ�Ͽٳ���
	cv::Mat cropImg = CropImage(img, rotated_rect);
	int flag = i++;
	//����ڰױ���1:1:3:1:1
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
	//roi = dst(RoiRect);
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
			/******************��ͼ��֪������ľ��ο��ռ�ܿ��3/7***********************/
			if (rect.width < mask.cols * 2 / 7)      //2/7��Ϊ�˷�ֹһЩ΢С�ķ���
				continue;
			if (Ratete(dstGray(rect)) > 0.75)       //0.75���Ҳ��Լ���ͼƬ�ľ���ֵ �ɸ����������(��������������)
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
	Mat src = srcImg.clone();
	int cols = srcImg.cols;
	int rows = srcImg.rows;
	vector<vector<Point>> qrPoint;
	vector<vector<Point>> contour2;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	cvtColor(srcImg, srcGray, COLOR_BGR2GRAY);
	threshold(srcGray, srcGray, 188, 255, THRESH_BINARY | THRESH_OTSU);
	//Mat edge;
	//Canny(srcGray, edge, 100, 255);
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
		//���������������Ƕ�ά��Ķ���
		if (ic >= 2)
		{
			bool isQr = QrPoint(contours[parentIdx], srcGray, parentIdx);
			//�����ҵ����ĸ���ɫ��λ��
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
			sprintf_s(tmp_name, "cor%d.png", i);
			//imwrite(tmp_name, image);
			if (isCorner(image))
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
		//warpPerspective(srcGray, output, warp_mat, srcImg.size());
		warpPerspective(src, output, warp_mat, srcImg.size());
		resize(output, output, Size(96, 96));
		//threshold(output, output, 150, 255, THRESH_BINARY | THRESH_OTSU);
		//cvtColor(output, output, COLOR_GRAY2BGR);//��ɫ�ָ�
		dst = output;
		//imwrite("final.png", output);
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

	a = (a < 128) ? 0 : 255;
	b = (b < 128) ? 0 : 255;
	c = (c < 128) ? 0 : 255;
	if (a == 0 && b == 0 && c == 0)
	{
		return 0;//��
	}
	else if (a == 255 && b == 255 && c == 255)
	{
		return 1;//��
	}
}

void Decode::getColorBits(Vec3b pix,vector<int> &bits)
{
	int b = pix[0];
	int g = pix[1];
	int r = pix[2];


	r = (r < 128) ? 0 : 255;
	g = (g < 128) ? 0 : 255;
	b = (b < 128) ? 0 : 255;


	if (r == 0 && g == 0 && b == 0)
	{
		bits.push_back(0);
		bits.push_back(0);
	}
	if (r == 0 && g == 0 && b == 255)
	{
		bits.push_back(0);
		bits.push_back(1);
	}
	if (r == 0 && g == 255 && b == 0)
	{
		bits.push_back(1);
		bits.push_back(0);
	}
	if (r == 255 && g == 0 && b == 0)
	{
		bits.push_back(1);
		bits.push_back(1);
	}
}

int Decode::getType(Mat& srcImg)
{
	int typecode = 0;//code=3
	int k = 1;//KΪ�����������ϵ��
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
	}//С�˷��ֽ�
}

int Decode::getLength(Mat& srcImg)
{
	int length = 0;
	int k = 1;
	for (int i = 0; i < 12; i++)//��16��[3,15]�����泤��
	{
		Vec3b pix = srcImg.at<Vec3b>(16, 4 + i);
		length += k * getBit(pix);
		k *= 2;
	}
	return length;
}


unsigned char* Decode::decode(Mat& dst, int& length, int& type)
{
	//Mat dst;
	//findQranchor(srcImg, dst);


	type = getType(dst);
	unsigned char* tmp = NULL;
	length = getLength(dst);
	//int change_flag = false;
	int tmplen;

	if (type == SINGLE || type == END)

	{
		tmplen = length;
		tmp = new unsigned char[tmplen];

	}
	else if (type == BEGIN||type==NORMAL)
	{
		tmplen = MAXSIZE;
		tmp = new unsigned char[tmplen];
	}
	for (int i = 0; i < tmplen; i++) { tmp[i] = 0; }
	//block A
	int index = 0;//�ݴ�������±�
/*	for (int i = 17; i < 80; i++)//������
	{
		for (int part = 0; part < 2; part++)//�����ֽ�
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
	}*/

	for (int i = 17; i < 80; i++)
	{
		for (int part = 0; part < 4; part++)
		{
			vector<int> bits;
			int code = 0;
			int k = 1;
			for (int j = 0; j < 4; j++)
			{
				Vec3b pix = dst.at<Vec3b>(i, j + part * 4);
				getColorBits(pix, bits);
			}
			for (int j = 0; j < 8; j++)
			{
				code += k * bits[j];
				k *= 2;
			}
			if (index >= tmplen) return tmp;
			tmp[index++] = (unsigned char)code;
		}
	}

	//block A�������

	//Block B����

	/*for (int i = 0; i < 16; i++)//������
	{
		for (int part = 0; part < 8; part++)//�����ֽ�
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
	*/

	for (int i = 0; i < 16; i++)//������
	{
		for (int part = 0; part < 16; part++)//�����ֽ�
		{
			int code = 0;
			int k = 1;
			vector<int> bits;
			for (int j = 0; j < 4; j++)
			{
				Vec3b pix = dst.at<Vec3b>(i, 16 + j + part * 4);
				getColorBits(pix, bits);
			}
			for (int j = 0; j < 8; j++)
			{
				code += k * bits[j];
				k *= 2;
			}
			if (index >= tmplen) return tmp;
			tmp[index++] = (unsigned char)code;
		}
	}

	//Block B�������

	//Block C���뿪ʼ//block3��4��5����

	/*for (int i = 16; i < 96; i++)//������
	{
		for (int part = 0; part < 10; part++)//�����ֽ�
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
	}*/
	//block3
	/*for (int i = 16; i < 96; i++)//������
	{
		for (int part = 0; part < 8; part++)//�����ֽ�
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
	}*/

	for (int i = 16; i < 96; i++)//������
	{
		for (int part = 0; part < 16; part++)//�����ֽ�
		{
			int code = 0;
			int k = 1;
			vector<int> bits;
			for (int j = 0; j < 4; j++)
			{
				Vec3b pix = dst.at<Vec3b>(i, 16+j + part * 4);
				getColorBits(pix, bits);
			}
			
			for (int j = 0; j < 8; j++)
			{
				code += k * bits[j];
				k *= 2;
			}

			if (index >= tmplen) return tmp;
			tmp[index++] = (unsigned char)code;
		}
	}





	/*for (int i = 16; i < 80; i++)//������
	{//block5
		for (int part = 8; part < 10; part++)//�����ֽ�
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
	}*/

	for (int i = 16; i < 80; i++)//������
	{//block5
		for (int part = 16; part < 20; part++)//�����ֽ�
		{
			int code = 0;
			int k = 1;
			vector<int> bits;
			for (int j = 0; j < 4; j++)
			{
				Vec3b pix = dst.at<Vec3b>(i, 16+j + part * 4);
				getColorBits(pix, bits);
			}

			for (int j = 0; j < 8; j++)
			{
				code += k * bits[j];
				k *= 2;
			}

			if (index >= tmplen) return tmp;
			tmp[index++] = (unsigned char)code;
		}
	}

	return tmp;

}

int Decode::getFlag(Mat& srcImg)
{
	int flagCode = 0;//code=3
	int k = 1;//KΪ�����������ϵ��
	for (int i = 2; i < 4; i++)
	{
		Vec3b pix = srcImg.at<Vec3b>(16, i);
		flagCode += k * getBit(pix);
		k *= 2;
	}
	return flagCode;
}
