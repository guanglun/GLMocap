#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QDateTime>

#define IMG_MAX_WIDTH   752
#define IMG_MAX_HIGH    480

#define IMG_FRAME_SIZE_MAX (40*1)
#define IMU_FRAME_SIZE_MAX 100

#define IMU_PACKAGE_SIZE 24

class Image : public QObject
{
    Q_OBJECT
public:
    unsigned char imu[IMU_FRAME_SIZE_MAX][IMU_PACKAGE_SIZE];
    unsigned char time[IMG_FRAME_SIZE_MAX][6];
    QDateTime qtime[IMG_FRAME_SIZE_MAX];
    unsigned char img[IMG_FRAME_SIZE_MAX][IMG_MAX_WIDTH*IMG_MAX_HIGH*2];
    unsigned char img_tmp[IMG_MAX_WIDTH*IMG_MAX_HIGH*2];
    unsigned char img_rgb[IMG_MAX_WIDTH*IMG_MAX_HIGH*3];
    int width,high,index,size;
    Image();
    unsigned char gs_bpp;
    void setImgSize(uint8_t frame_size_num);
    bool recv(unsigned char* data,int len);
    void CONVERT_YUV420PtoRGB24(unsigned char* yuv_src,unsigned char* rgb_dst,int nWidth,int nHeight);
private:
    unsigned char CONVERT_ADJUST(double tmp);    
};

#endif // IMAGE_H
