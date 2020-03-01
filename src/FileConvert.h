#include<iostream>
#include<ctime>
#include<cstdio>
#include<string>
using namespace std;
class FileConvert
{
private:
	FILE* p;
	unsigned char* dst;
	void GenerateRandomArray(unsigned char* src, unsigned long size);
public:
	~FileConvert();
	unsigned char* FileToByte(char filename[], unsigned long* size);
	void ByteToFile(unsigned char* src, char filename[], unsigned long size);
	void GenerateRandFile(char filename[], unsigned long size);
	void PicTransToVideo(int picNumPersec);
	void VideoTransToPic();
	unsigned long GetFileSize(FILE* p);
};