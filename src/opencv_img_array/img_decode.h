//传入图片地址，输出二维vector
vector<vector<uchar> > img_decode(char* path)   //path为图片路径
{
    Mat img = imread(path);                // 将图片传入Mat容器中
//       显示原图片
//       namedWindow("old", WINDOW_NORMAL);
//       imshow("old", img);
//      waitKey(0);
    int w = img.cols * img.channels();     //可能为3通道，宽度要乘图片的通道数
    int h = img.rows;

    vector<vector<uchar> > array(h, vector<uchar>(w));      //初始化二维vector
    for (int i = 0; i < h; i++)
    {
        uchar* inData = img.ptr<uchar>(i);            //ptr为指向图片的行指针，参数i为行数
        for (int j = 0; j < w; j++)
        {
            array[i][j] = inData[j];
        }
    }
    return array;
}
/*
//传入图片地址，输出数组
void decode(char* path)   //path为图片路径，buf为传出数组
{
    Mat img = imread(path);
 //   namedWindow("old", WINDOW_NORMAL);
//    imshow("old", img);
 //   waitKey(0);
    int w = img.cols*img.channels();     //可能为3通道
    int h = img.rows;


    if (img.isContinuous())     //如果是连续的则可以，把整个图像完全展开看成一行
    {
        w = w * h;
        h = 1;              //变为一行
    }
    uchar** buf = new uchar * [h];
    for (int i = 0; i < h; i++)
        buf[i] = new uchar[w];
    for (int i = 0; i < h; i++)
    {
        uchar* inData = img.ptr<uchar>(i);
        for (int j = 0; j < w; j++)
        {
            buf[i][j]= inData[j];
        }
    }
    for (int i = 0; i < h; i++)
      delete[] buf[i];
    delete[] buf;
}
*/