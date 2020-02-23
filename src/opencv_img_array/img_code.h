
//传入二维vecotr，显示输出的图片，并保存图片到指定地址
void img_code(vector<vector<uchar> > array, char* path)
{
    size_t h = array.size();                  
    size_t w = array[0].size();
    //初始化图片的像素长宽
    Mat img(h, (size_t)(w/3), CV_8UC3);           //保存为RGB，图像列数像素要除以3；
    for (size_t i = 0; i < h; i++)
    {
        uchar* outData = img.ptr<uchar>(i);
        for (size_t j = 0; j < w; j++)
        {
             outData[j] = array[i][j];
        }
    }
    namedWindow("new", WINDOW_NORMAL);
    imshow("new", img);
    waitKey(0);
    imwrite("目标地址\\图片", img);         //输出图片
}