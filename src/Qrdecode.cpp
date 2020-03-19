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
	/*
	先把所有图片转换为ROI

	*/
	int count = 1;
	Mat img;
	Mat dst;
	int foundindex = 0;
	while (true)
	{
		sprintf_s(fileName, "imageOutput\\%05d.png", count++);
		img = imread(fileName);
		if (img.data == NULL) break;
		int found = dec.findQranchor(img, dst);
		if (found == 1)
		{
			foundindex = count - 1;
			sprintf_s(tmpName, "imageTmp\\%05d.png", foundindex);
			imwrite(tmpName, dst);
			cout << "find anchor " << fileName << endl;
		}
		else
		{
			cout << "not found" << fileName << endl;
		}
	}
	
	//遍历每张图片，检测同步码是否发生变化
		//同时过滤掉残影图
		//将有需要的名字传入传入array中，获取二维码张树
	vector<string> fileNames;
	int currentFlag = 0;
	int originFlag = 0;
	char beginPic[120];
	sprintf_s(fileName, "imageTmp\\%05d.png", 1);
	fileNames.push_back(fileName);
	img = imread(fileName);
	currentFlag = dec.getFlag(img);
	originFlag = currentFlag;
	for (int i = 2; i <= foundindex; i++)
	{
		sprintf_s(fileName, "imageTmp\\%05d.png", i);
		img = imread(fileName);
		if (img.data == NULL) continue;
		currentFlag = dec.getFlag(img);
		if (currentFlag==2&&originFlag==1|| currentFlag == 1 && originFlag == 2)
		{
			if (dec.getRate(img) - 0.51 < 0)//如果不是残影
			{
				fileNames.push_back(fileName);
				originFlag = currentFlag;
			}
		}
		else
		{
			continue;
		}
	}

	int numOfPic = fileNames.size();//converter.GetFilesNumber(path);
	cout << "num of true pic " << numOfPic << endl;
	char filename[10];
	img = imread(fileNames[numOfPic-1]);//最后张图片单独处理
	int length;
	vector<int> Binary;
	unsigned char* output = NULL;
	int type;
	dec.decode(img, length, type,Binary);//为了获取最后一张的长度
	int total_length = (numOfPic - 1) * 1022 + length;
	char outfilename[] = "out.bin";
	count=0;//图像文件计数
	

	cout << total_length << endl;
	cout << indexptr;

	if (type == SINGLE)//如果只有单张图
	{
		//length为总文件的有效数据长度
	}//正常退出
	else
	{
		//length为总文件的长度
		Binary.clear();
	}

	count = 0;
	int tmplen;

	while (true)
	{
		if (count == numOfPic) break;
		img = imread(fileNames[count++]);//读入图像；
		dec.decode(img, tmplen, type,Binary);
		if (tmplen > 1022) tmplen = 1022;
		cout << "dealing with " << fileNames [count-1]<< " "<<tmplen << endl;
	}

	output = dec.binToDec(Binary);

	converter.ByteToFile(output, outfilename, total_length);

	delete[]output;
}