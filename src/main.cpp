//#include<iostream>
//#include"Decode.h"
//#include"FileConvert.h"
//#include<cstdio>
//#include<cstdlib>
//#include"code.h"
//using namespace std;
//using namespace cv;
//
//int main()
//{
//
//	FileConvert converter;
//	string path = "imageSrc\\";
//	int function;
//	cout << "function one:encode file to video" << endl;
//	cout << "function two:decode video to file" << endl;
//	cout << "Calculate bit error rate" << endl;
//	cout << "input function you choose" << endl;
//	cin >> function;
//	if (function == 1)
//	{
//		//选择输入字符串or生成随机比特流//根据字符串生成文件的接口还没有写
//		unsigned long size;
//		cout << "Please input size :";
//		cin >> size;//需要生成文件的大小，单位字节
//		char filename[] = "in.bin";
//		converter.GenerateRandFile(filename, size);//生成随机二进制文件
//		unsigned char* bytes = converter.FileToByte(filename, &size);//读取文件生成的Byte数组
//		
//		//一段由bytes生成图像的代码
////		命名格式为%05d.png
////		输出在imageSrc目录
////		
//		Code::Main(bytes, size, "imageSrc\\", "png");
//		int fps = 10;//测试用每秒10帧试试
//		converter.PicTransToVideo(fps);
//		return 0;
//	}
////
////	用手机录制视频并拍摄
//
//	else if (function == 2)
//	{
//		int indexptr = 0;//用于存储目的数组的当前下标
//		Decode decoder;
//		int length;
//		converter.VideoTransToPic();
//		unsigned char* output = NULL;
//		int numOfPic = converter.GetFilesNumber(path);//获取imageOutput目录下文件个数
//		Mat img;
//
//		char filename[10];
//		sprintf_s(filename, "%05d.png", numOfPic);//最后一张
//		string path = "imageOutput\\";
//		img = imread(path +filename);//最后张图片单独处理
//
//		int type;
//		unsigned char *tmp = decoder.decode(img, length, type);//为了获取最后一张的长度
//
//		int total_length = (numOfPic-1)*1054+length;
//
//
//
//		cout << total_length << endl;
//
//		cout << indexptr;
//
//		if (type == SINGLE)//如果只有单张图
//		{
//			output = tmp;//length为总文件的有效数据长度
//		}//正常退出
//		else //if (type == BEGIN)//多张图片的第一张图片
//		{
//			//length为总文件的长度
//			int tmplen=MAXSIZE;
//			output = new unsigned char[total_length];//用于储存数据的数组
//			//memcpy(output + indexptr, tmp, tmplen);//复制到最终的数组里
//			//indexptr += tmplen;
//			delete[]tmp;
//		}//不可能是end和normal了
//
//		for (int i = 1; i <= numOfPic; i++)
//		{
//			Mat img;
//			string path = "imageOutput\\";
//			char filename[10];
//			sprintf_s(filename, "%05d.png", i);
//			img = imread(path + filename);
//			if (img.empty())
//			{
//				cout << "fail to open";
//				return 0;
//			}
//			int tmplen;
//			unsigned char* tmp = decoder.decode(img,tmplen, type);//将当前二维码的数据暂存于数组中
//			if (tmplen > 1054) tmplen = 1054;
//			cout << filename << " " << tmplen << endl;
//			memcpy(output + indexptr, tmp, tmplen);//复制到最终的数组里
//			indexptr += tmplen;
//			delete[]tmp;
//		}//遍历所有图片
//		char outfilename[] = "out.bin";
//		converter.ByteToFile(output, outfilename, 10240);
//	}
//	else if (function == 3)
//	{
//		FileConvert file;
//		FILE* fp1, * fp2;
//		unsigned long num1 = 0, num2 = 0;
//		fopen_s(&fp1, "in.bin", "rb");
//		fopen_s(&fp2, "out.bin", "rb");
//		num1 = file.GetFileSize(fp1);
//		num2 = file.GetFileSize(fp1);
//		unsigned char* dst1 = new unsigned char[num1];
//		unsigned char* dst2 = new unsigned char[num2];
//		fread(dst1, sizeof(unsigned char), num1, fp1);
//		fread(dst2, sizeof(unsigned char), num1, fp2);
//		unsigned long totalnum = 1, wrongnum = 0, m = 0;
//		while (totalnum <= (num1 + 1))
//		{
//			totalnum++;
//			if (dst1[m] != dst2[m]) wrongnum++;
//			m++;
//		}
//		double n;
//		n = 1 - (wrongnum / totalnum);
//		cout << "bit error rate is " << n << endl;
//		return 0;
//	}
//}