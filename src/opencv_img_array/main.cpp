#include<vector>
#include <opencv2/opencv.hpp>
//#include "opencv2/highgui/highgui.hpp" 
//#include "opencv2/core/core.hpp"
using namespace std;
using namespace cv;
#include"img_decode.h"
#include"img_code.h"

int main()
{
    char readPath[] = "读入地址\\图片";    //传入图片的地址

    vector<vector<uchar> > array;
    array = img_decode(readPath);

    char writePath[] = "目标地址";        //输出图片的保存位置
    img_code(array, writePath);

    return 0;
}