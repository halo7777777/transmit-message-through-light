#include<iostream>
#include"Decode.h"
#include"FileConvert.h"
#include<cstdio>
#include<cstdlib>
//#include"code.h"

#include<algorithm>
#include"Qrdecode.h"

using namespace std;
using namespace cv;

int main(char argc, char* argv[])
{/*
	cout << "This is the decode program." << endl;
	//cout << "Please enter the command" << endl;*/
	//char takedecode[20];//�����ַ���decode
	//char outputname[20]=argv[1];//�ֻ��������Ƶ����
	//char outfile[20];//����Ķ������ļ�
	//char outfiletest[20];//�����У���ļ�
	//cin >> takedecode >> outputname >> outfile >> outfiletest;
	Qrdecode mydecode;
	mydecode.mydecode(argv[1],argv[2],argv[3]);
}