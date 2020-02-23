#include<iostream>
#include<ctime>
#include<cstdio>
using namespace std;
class FileConvert
{
private:
	FILE* p;
	unsigned char* dst;
	unsigned long GetFileSize(FILE* p);
	void GenerateRandomArray(unsigned char* src, unsigned long size);
public:
	~FileConvert();
	unsigned char* FileToByte(char filename[], unsigned long* size);
	void ByteToFile(unsigned char* src, char filename[], unsigned long size);
	void GenerateRandFile(char filename[], unsigned long size);
};