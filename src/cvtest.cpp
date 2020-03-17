//#include<opencv2/opencv.hpp>
//#include"Decode.h"
//using namespace std;
//int main()
//{
//	Mat src;
//	Mat dst;
//	src = imread("src.png");
//	dst = imread("dst.png");
//	Mat out;
//	bitwise_xor(src, dst, out);
//	imwrite("out.png", out);
//	return 0;
//	Decode dec;
//	//dec.findQranchor(src, dst);
//	//imshow("test", dst);
//	//waitKey(0);
//	int typecode = 0;//code=3
//	int k = 1;//K为二进制运算的系数
//
//	vector<int> bits;
//
//	for (int i = 0; i < 16; i++)
//	{
//		Vec3b pix = src.at<Vec3b>(17, i);
//		dec.getColorBits(pix, bits);
//	}
//	
//	cout << "-------------------------------------" << endl;
//	for (int i = 0; i < bits.size(); i+=2)
//	{
//		cout << bits[i] << bits[i + 1] << endl;
//	}
//
//
//}