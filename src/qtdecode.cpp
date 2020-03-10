#include "qtdecode.h"
#include"Decode.h"
#include"FileConvert.h"
#include<iostream>
using namespace std;
using namespace cv;

void qtdecode::mydecode()
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
	int foundindex = 1;
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
			cout << "not found" << fileName;
		}
	}

	//遍历每张图片，检测同步码是否发生变化
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
		currentFlag = dec.getFlag(img);
		if (currentFlag==2&&originFlag==1|| currentFlag == 1 && originFlag == 2)
		{
			fileNames.push_back(fileName);
			originFlag = currentFlag;
		}
		else
		{
			continue;
		}

	}



	int numOfPic = fileNames.size();//converter.GetFilesNumber(path);
	cout << "num of true pic " << numOfPic << endl;
	char filename[10];
	///sprintf_s(filename, "%05d.png", numOfPic);//最后一张
	img = imread(fileNames[numOfPic-1]);//最后张图片单独处理
	int length;
	unsigned char* output = NULL;
	int type;
	unsigned char* tmp = dec.decode(img, length, type);//为了获取最后一张的长度
	int total_length = (numOfPic - 1) * 1022 + length;
	//Mat image;//源图片
	int i, j;
	char outfilename[] = "out.bin";
	count=0;//图像文件计数
	

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

	while (true)
	{
		if (count == numOfPic) break;
		//sprintf_s(fileName, "imageOutput\\%05d.png", count++);
		img = imread(fileNames[count++]);//读入图像；
		//if (count > numOfPic||img.data==NULL) break;
		//dec.findQranchor(image, dst);
		/*for (i = 0; i <= 96; i++)
		{
			for (j = 0; j <= 96; j++)
			{
				output
				fwrite;
			}
		}*/
		unsigned char* tmp = dec.decode(img, tmplen, type);
		if (tmplen > 1022) tmplen = 1022;
		cout << "dealing with " << fileNames [count-1]<< " "<<tmplen << endl;
		memcpy(output + indexptr, tmp, tmplen);//复制到最终的数组里
		indexptr += tmplen;
		delete[]tmp;
	}

	converter.ByteToFile(output, outfilename, total_length);

}