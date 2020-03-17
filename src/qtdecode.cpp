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
	int indexptr = 0;//���ڴ洢Ŀ������ĵ�ǰ�±�
	Decode dec;
	string path = "imageOutput\\";
	FileConvert converter;
	converter.VideoTransToPic();
	/*
	�Ȱ�����ͼƬת��ΪROI

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
	
	foundindex = 49;
	//����ÿ��ͼƬ�����ͬ�����Ƿ����仯
		//������Ҫ�����ִ��봫��array�У���ȡ��ά������
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
	///sprintf_s(filename, "%05d.png", numOfPic);//���һ��
	img = imread(fileNames[numOfPic-1]);//�����ͼƬ��������
	int length;
	unsigned char* output = NULL;
	int type;
	unsigned char* tmp = dec.decode(img, length, type);//Ϊ�˻�ȡ���һ�ŵĳ���
	int total_length = (numOfPic - 1) * MAXSIZE + length;
	//Mat image;//ԴͼƬ
	int i, j;
	char outfilename[] = "out.bin";
	count=0;//ͼ���ļ�����
	

	cout << total_length << endl;
	cout << indexptr;

	if (type == SINGLE)//���ֻ�е���ͼ
	{
		output = tmp;//lengthΪ���ļ�����Ч���ݳ���
	}//�����˳�
	else //if (type == BEGIN)//����ͼƬ�ĵ�һ��ͼƬ
	{
		//lengthΪ���ļ��ĳ���
		int tmplen = MAXSIZE;
		output = new unsigned char[total_length];
		delete[]tmp;
	}

	count = 0;
	int tmplen;

	while (true)
	{
		if (count == numOfPic) break;
		img = imread(fileNames[count++]);//����ͼ��
		unsigned char* tmp = dec.decode(img, tmplen, type);
		if (tmplen > MAXSIZE) tmplen = MAXSIZE;
		cout << "dealing with " << fileNames [count-1]<< " "<<tmplen << endl;
		memcpy(output + indexptr, tmp, tmplen);//���Ƶ����յ�������

		cout << endl;
		indexptr += tmplen;
		delete[]tmp;
	}


	converter.ByteToFile(output, outfilename, total_length);

}