#include "Qrdecode.h"
#include"Decode.h"
#include"FileConvert.h"
#include<iostream>
using namespace std;
using namespace cv;


void Qrdecode::mydecode()
{
	char fileName[128];
	char tmpName[128];
	int indexptr = 0;//用于存储目的数组的当前下标
	Decode dec;
	string path = "imageOutput\\";
	FileConvert converter;
	converter.VideoTransToPic();

	vector<Mat> dstTmp;

	/*
	先把所有图片转换为ROI
	*/
	int count = 1;
	Mat img;
	Mat dst;
	int foundindex = 85;
	while (true)
	{
		sprintf_s(fileName, "imageOutput\\%05d.png", count++);
		img = imread(fileName);
		if (img.data == NULL) break;
		int found = dec.findQranchor(img, dst);
		if (found == 1)
		{
			foundindex = count - 1;
			dstTmp.push_back(dst);
		}
		else
		{
		}
	}

	//foundindex = 87;
	//遍历每张图片，检测同步码是否发生变化
		//将有需要的名字传入传入array中，获取二维码张树
	vector<unsigned char> valid;
	int currentFlag = 0;
	int originFlag = 0;
	char beginPic[120];
	vector<int> validIndex;
	validIndex.push_back(0);//第0张图片
	img = dstTmp[0];

	currentFlag = dec.getFlag(img);
	originFlag = currentFlag;
	for (int i = 1; i < foundindex; i++)
	{
		img = dstTmp[i];
		if (img.data == NULL) continue;
		currentFlag = dec.getFlag(img);
		if (currentFlag == 2 && originFlag == 1 || currentFlag == 1 && originFlag == 2)
		{
			if (dec.getRate(img) - 0.51 < 0)
			{
				validIndex.push_back(i);//下标为i的图片有效
				originFlag = currentFlag;
			}
		}
		else
		{
			continue;
		}

	}



	int numOfPic = validIndex.size();//converter.GetFilesNumber(path);
	cout << "num of true pic " << numOfPic << endl;
	char filename[10];
	img = dstTmp[validIndex[numOfPic-1]];//最后张图片单独处理
	int length;
	unsigned char* output = NULL;
	int type;
	unsigned char* tmp = dec.decode(img, length, type, valid);//为了获取最后一张的长度
	int total_length = (numOfPic - 1) * 1018 + length;
	//Mat image;//源图片
	int i, j;
	char outfilename[] = "out.bin";
	count = 0;//图像文件计数


	cout << total_length << endl;
	cout << indexptr;

	if (type == SINGLE)//如果只有单张图
	{
		output = tmp;//length为总文件的有效数据长度
	}//正常退出
	else //if (type == BEGIN)//多张图片的第一张图片
	{
		//length为总文件的长度
		int tmplen = MAXSIZE;
		output = new unsigned char[total_length];//用于储存数据的数组
		//memcpy(output + indexptr, tmp, tmplen);//复制到最终的数组里
		//indexptr += tmplen;
		delete[]tmp;
	}//不可能是end和normal了

	count = 0;
	int tmplen;
	valid.clear();
	while (true)
	{
		if (count == numOfPic) break;
		img = dstTmp[validIndex[count++]];//读入图像；
		unsigned char* tmp = dec.decode(img, tmplen, type, valid);
		if (tmplen > 1018) tmplen = 1018;
		memcpy(output + indexptr, tmp, tmplen);//复制到最终的数组里
		indexptr += tmplen;
		delete[]tmp;
	}
	while (valid.size() < total_length)
	{
		valid.push_back(0xff);
	}
	converter.ByteToFile(output, outfilename, total_length);
	unsigned char* v = new unsigned char[valid.size() * sizeof(unsigned char)];
	memcpy(v, &valid[0], valid.size() * sizeof(unsigned char));
	converter.ByteToFile(output, outfilename, total_length);

	int total_byte = valid.size();
	int total_bit = valid.size() * 8;
	int giveUp = 0;
	for (int i = 0; i < total_byte; i++)
	{
		if (valid[i] == 0)
		{
			giveUp += 8;
		}
	}

	cout << giveUp << endl; 


	char vfname[] = "v.bin";
	converter.ByteToFile(v, vfname, total_length);
} 