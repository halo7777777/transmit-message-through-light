#include<iostream>
#include<cstdio>
#include<cstdlib>
#include"encode.h"
#include"FileConvert.h"
#include<algorithm>

using namespace std;
using namespace cv;

int main(char argc, char* argv[])
{
	//cout << "This is the encode program." << endl;
	//cout << "Please enter the command" << endl;
	char encodetake[20];//用于接受字符串encode
	char filename[20];//输出二进制文件名称
	char outvideo[20];//输出视频的名称
	unsigned int videolength_ms;//视频长度
	//cin >> encodetake >> filename >> outvideo >> videolength_ms;
	double times;
	times = atoi(argv[3]) / 1000.0;
	unsigned long size;
	unsigned int fps = 25;
	unsigned int bitofeachpicture = 1018;
	size = (int)(times * fps * bitofeachpicture);

	unsigned char* bytes = NULL;

	FileConvert converter;
	converter.GenerateRandFile(argv[1], size);
	bytes = converter.FileToByte(argv[1], &size);//读取文件生成的Byte数组

	Code::Main(bytes, size, "imageSrc\\", "png", 1);
	converter.PicTransToVideo(fps,argv[2]);
}