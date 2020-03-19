#include"code.h"
namespace Code
{

    constexpr unsigned long BytesPerFrame = 1018;    //每帧图的字节数
    constexpr int FrameSize = 100;
    constexpr int FrameOutputRate = 10;
    constexpr int SafeAreaWidth = 2;
    constexpr int BPatternSize = 18;
    constexpr int areanum = 5;
    const Vec3b pixel[8] =
    {
        Vec3b(0,0,0),Vec3b(0,0,255),Vec3b(0,255,0),Vec3b(255,0,0),
        Vec3b(0,255,255),Vec3b(255,0,255),Vec3b(255,255,0),Vec3b(255,255,255)
    };
    const unsigned long len_max[areanum] = { 124,128,512,128,126 };
    const int areapos[areanum][2][2] =
    {//[2][2],第一维度代表高宽，第二维度代表左上角坐标
        {{62,16},{BPatternSize + 1,SafeAreaWidth}},
        {{16,64},{SafeAreaWidth,BPatternSize}},
        {{64,64},{BPatternSize,BPatternSize}},
        {{16,64},{FrameSize - BPatternSize,BPatternSize}},
        {{63,16},{BPatternSize,FrameSize - BPatternSize}}
    };
    enum color
    {
        Black = 0,
        White = 7
    };
    enum class FrameType
    {
        Start = 0,
        Normal = 1,
        End = 2,
        Single = 3,

    };
    void Main(unsigned char* info, unsigned long len, const char* savePath, const char* outputFormat, int tag) // 字符串信息，长度，保存路径，保存格式
    {
        Mat output;
        char fileName[128];
        int count = 0;
        int count_test = 0;
        if (len <= BytesPerFrame)
        {
            unsigned char BUF[BytesPerFrame + 5];
            memcpy(BUF, info, sizeof(unsigned char) * len);
            for (int i = len; i <= BytesPerFrame; ++i)
                BUF[i] = rand() % 256;
            if (tag == 0)
            {
                output = amplify(transform(CodeFrame(FrameType::Single, BUF, len, 0)), 0);
                sprintf_s(fileName, "%s\\test%05d.%s", savePath, count_test++, outputFormat);
                imwrite(fileName, output);
            }
            output = amplify(CodeFrame(FrameType::Single, BUF, len, 0), 1);
            sprintf_s(fileName, "%s\\%05d.%s", savePath, count++, outputFormat);
            imwrite(fileName, output);
        }
        else
        {
            int PicNum = 0;
            if (tag == 0)
            {
                output = amplify(transform(CodeFrame(FrameType::Start, info, len, PicNum++)), 0);
                sprintf_s(fileName, "%s\\test%05d.%s", savePath, count_test++, outputFormat);
                imwrite(fileName, output);
            }
            output = amplify(CodeFrame(FrameType::Start, info, len, PicNum++), 1);
            sprintf_s(fileName, "%s\\%05d.%s", savePath, count++, outputFormat);
            imwrite(fileName, output);
            do
            {
                len -= BytesPerFrame;
                info += BytesPerFrame;
                if (len > BytesPerFrame)
                {
                    if (tag == 0)
                    {
                        output = amplify(transform(CodeFrame(FrameType::Normal, info, BytesPerFrame, PicNum++)), 0);
                        sprintf_s(fileName, "%s\\test%05d.%s", savePath, count_test++, outputFormat);
                        imwrite(fileName, output);
                    }
                    output = amplify(CodeFrame(FrameType::Normal, info, BytesPerFrame, PicNum++), 1);
                    sprintf_s(fileName, "%s\\%05d.%s", savePath, count++, outputFormat);
                    imwrite(fileName, output);
                }
                else
                {
                    unsigned char BUF[BytesPerFrame + 5];
                    memcpy(BUF, info, sizeof(unsigned char) * len);
                    for (int i = len; i <= BytesPerFrame; ++i)
                        BUF[i] = rand() % 256;
                    if (tag == 0)
                    {
                        output = amplify(transform(CodeFrame(FrameType::End, BUF, len, PicNum++)), 0);
                        sprintf_s(fileName, "%s\\test%05d.%s", savePath, count_test++, outputFormat);
                        imwrite(fileName, output);
                    }
                    output = amplify(CodeFrame(FrameType::End, BUF, len, PicNum++), 1);
                    sprintf_s(fileName, "%s\\%05d.%s", savePath, count++, outputFormat);
                    imwrite(fileName, output);
                }
                /*              测试是否能用于定使
                                int i = 0;
                                Mat dst = imread(fileName);
                                QRCodeDetector qrDetector;
                                vector<Point2f> list;
                                qrDetector.detect(dst, list);
                                if (list.empty())
                                {
                                    cout << i <<" nothing" << endl;
                                }
                                i++;
                */
            } while (len > BytesPerFrame);
        }
        return;
    }
    Mat amplify(const Mat& src, int tag)
    {
        Mat output;
        int FrameOutputSize = FrameSize * FrameOutputRate;
        if (tag == 0)
            FrameOutputSize = (FrameSize - 2 * SafeAreaWidth) * FrameOutputRate;
        output = Mat(FrameOutputSize, FrameOutputSize, CV_8UC3);
        for (int i = 0; i < FrameOutputSize; ++i)
        {
            for (int j = 0; j < FrameOutputSize; ++j)
            {
                output.at<Vec3b>(i, j) = src.at<Vec3b>(i / FrameOutputRate, j / FrameOutputRate);
            }
        }
        return output;
    }
    Mat CodeFrame(FrameType frameType, unsigned char* info, unsigned long tailLen, int PicNum)
    {
        Mat codeMat = Mat(FrameSize, FrameSize, CV_8UC3, Vec3b(255, 255, 255));     //底片为白色
        if (frameType == FrameType::Start || frameType == FrameType::Normal)
            //3/1/14:30决定不存朿大长度，朿大长度由朿后一张长庿+BytesPerFrame*张数计算      
            tailLen = BytesPerFrame;
        BulidQrPoint(codeMat);        //绘制定位码       
        BulidFrameFlag(codeMat, frameType, tailLen);
        BulidPicNum(codeMat, PicNum);
        if (tailLen != BytesPerFrame)           //编码时整张图都要编码，未确定的是随机敿
            tailLen = BytesPerFrame;
        int len_CRC = 0;         //用于计算CRC校验码的数据长度
        unsigned char* info_CRC = info;      //用于计算CRC校验码的数据
        for (int i = 0; i < areanum && tailLen > 0; ++i)
        {
            int len_now = std::min(tailLen, len_max[i]);
            BulidInfoRect(codeMat, info, len_now, i);
            tailLen -= len_now;
            len_CRC += len_now;
            if (i == 0 || i == 4)          //区域1和区域2，3，4，5进行校验
            {
                BuildCRC_16(codeMat, info_CRC, len_CRC, i);
            }
            info += len_now;
        }
        return codeMat;
    }
    void BulidQrPoint(Mat& mat)
    {
        //绘制大二维码识别炿
        constexpr int pointPos[4][2] =
        {
            {0,0},
            {0,FrameSize - BPatternSize},
            {FrameSize - BPatternSize,0},
            {FrameSize - BPatternSize,FrameSize - BPatternSize}
        };
        const Vec3b vec3bBig[9] =
        {
            pixel[Black],
            pixel[Black],
            pixel[Black],
            pixel[White],
            pixel[White],
            pixel[Black],
            pixel[Black],
            pixel[White],
            pixel[White]
        };
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < BPatternSize; ++j)
                for (int k = 0; k < BPatternSize; ++k)
                    mat.at<Vec3b>(pointPos[i][0] + j, pointPos[i][1] + k) =
                    vec3bBig[(int)max(fabs(j - 8.5), fabs(k - 8.5))];       //打印回字
    }
    void BulidInfoRect(Mat& mat, unsigned char* info, unsigned long len, int areaID)
    {
        const unsigned char* pos = (const unsigned char*)info;
        const unsigned char* end = pos + len;
        for (int i = 0; i < areapos[areaID][0][0]; ++i)
        {
            uint32_t outputCode = 0;
            for (int j = 0; j < areapos[areaID][0][1] / 8; ++j)    // 1 char = 8 bits
            {
                outputCode |= *pos++;
                /*
                for (int k = 0; k < 2; ++k)
                {
                    outputCode <<= 8;
                    if (pos != end)
                        outputCode |= *pos++;
                }
                */
                for (int k = areapos[areaID][1][1]; k < areapos[areaID][1][1] + 8; ++k)
                {
                    //  mat.at<Vec3b>(i+areapos[areaID][1][0], j*8+k) = pixel[outputCode&3];
                    //  outputCode >>= 2;
                    mat.at<Vec3b>(i + areapos[areaID][1][0], j * 8 + k) = pixel[(outputCode & 1) ? 7 : 0];
                    outputCode >>= 1;
                }
                if (pos == end) break;
            }
            if (pos == end) break;
        }
    }
    void BulidFrameFlag(Mat& mat, FrameType frameType, unsigned long tailLen)
    {
        switch (frameType)
        {
        case FrameType::Start:
            mat.at<Vec3b>(BPatternSize, SafeAreaWidth) = pixel[Black];
            mat.at<Vec3b>(BPatternSize, SafeAreaWidth + 1) = pixel[Black];
            break;
        case FrameType::Normal:
            mat.at<Vec3b>(BPatternSize, SafeAreaWidth) = pixel[Black];
            mat.at<Vec3b>(BPatternSize, SafeAreaWidth + 1) = pixel[White];
            break;
        case FrameType::End:
            mat.at<Vec3b>(BPatternSize, SafeAreaWidth) = pixel[White];
            mat.at<Vec3b>(BPatternSize, SafeAreaWidth + 1) = pixel[Black];
            break;
        case FrameType::Single:
            mat.at<Vec3b>(BPatternSize, SafeAreaWidth) = pixel[White];
            mat.at<Vec3b>(BPatternSize, SafeAreaWidth + 1) = pixel[White];
            break;
        }
        for (int i = 4; i < 16; ++i)
        {
            mat.at<Vec3b>(BPatternSize, SafeAreaWidth + i) = pixel[(tailLen & 1) ? 7 : 0];
            tailLen >>= 1;
        }
    }
    void BulidPicNum(Mat& mat, int PicNum)
    {
        mat.at<Vec3b>(BPatternSize, SafeAreaWidth + 2) = pixel[(PicNum & 1) ? 0 : 7];
        mat.at<Vec3b>(BPatternSize, SafeAreaWidth + 3) = pixel[(PicNum & 1) ? 7 : 0];
    }
    Mat transform(const Mat& mat)         //切割为96×96
    {
        Mat output;
        constexpr int FrameOutputSize = FrameSize - 2 * SafeAreaWidth;
        output = Mat(FrameOutputSize, FrameOutputSize, CV_8UC3);
        for (int i = SafeAreaWidth; i < FrameOutputSize + SafeAreaWidth; ++i)
        {
            for (int j = SafeAreaWidth; j < FrameOutputSize + SafeAreaWidth; ++j)
            {
                //         printf("%d %d\n",i,j);
                output.at<Vec3b>(i - SafeAreaWidth, j - SafeAreaWidth) = mat.at<Vec3b>(i, j);
            }
        }
        return output;
    }
    //打表求校验码
    unsigned int crc_ta_8[256] =
    { /* CRC 字节余式表 */
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
        0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
        0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
        0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
        0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
        0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
        0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
        0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
        0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
        0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
        0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
        0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
        0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
        0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
        0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
        0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
        0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
        0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
        0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
        0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
        0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
        0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
        0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
        0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
        0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
        0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
        0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
        0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
        0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
        0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
        0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
        0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
    };
    void BuildCRC_16(Mat& mat, unsigned char* info, int len, int area_No)         //CRC-16/CCITT        x16+x12+x5+1 
    {
        uint16_t crc = 0;
        //得到crc
        while (len-- != 0)
        {
            unsigned int high = (unsigned int)(crc / 256); //取CRC高8位
            crc <<= 8;
            crc ^= crc_ta_8[high ^ *info];
            info++;
        }
  //      printf("%x\n", crc);
        //写入二维码图像中
        for (int i = 0; i < 16; i++)
        {
            if (area_No == 0)
            {
                mat.at<Vec3b>(FrameSize - BPatternSize-1, SafeAreaWidth + i) = pixel[(crc & 1) ? 7 : 0];
            }
            else if (area_No == 4)
            {
                mat.at<Vec3b>(FrameSize - BPatternSize-1, FrameSize - BPatternSize + i) = pixel[(crc & 1) ? 7 : 0];
            }
            crc >>= 1;
        }
    }
}