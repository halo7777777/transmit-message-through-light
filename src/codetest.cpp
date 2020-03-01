/*
#include"code.h"

int main()
{
    
    FILE* p;
    fopen_s(&p,"C:\\Users\\18705996097\\Desktop\\111\\in.bin", "rb");
    unsigned long current_read_position = ftell(p);
    fseek(p, 0, SEEK_END);
    unsigned long size = ftell(p);
    fseek(p, current_read_position, SEEK_SET);



    unsigned char* dst = new unsigned char[size];
    fread(dst, sizeof(unsigned char), size, p);
    fclose(p);



    Code::Main(dst, size, "C:\\Users\\18705996097\\Desktop\\111", "png");


    delete[]dst;

    return 0;
}
*/