#include "CamProcess.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

CamProcess::CamProcess(OPENVIO *vio, QObject *parent)
{
    this->vio = vio;
}

void CamProcess::setShowFlag(Qt::CheckState flag)
{
    showFlag = flag;
}

void CamProcess::camSlot(int index)
{
    // static uint32_t timer;
    // static uint32_t t1,t1_old;
    // static uint16_t t2,t2_old;
    // static bool is_first = true;
    // static float d_time = 0;

    QImage myImage;
    QPixmap pixImage;
    // QDateTime timeNow;

    // t1 = (uint32_t)(vio->img.time[index][0]<<24);
    // t1 |= (uint32_t)(vio->img.time[index][1]<<16);
    // t1 |= (uint32_t)(vio->img.time[index][2]<<8);
    // t1 |= (uint32_t)(vio->img.time[index][3]<<0);

    // t2 = (uint16_t)(vio->img.time[index][4]<<8);
    // t2 |= (uint16_t)(vio->img.time[index][5]<<0);

    // if(is_first == true)
    // {
    //     is_first = false;
    //     t1_old = t1;
    //     t2_old = t2;
    //     return;
    // }

    // if(t2 > t2_old)
    // {
    //     timer = t2-t2_old;
    // }else{
    //     timer = (uint32_t)t2 + 50000 - t2_old;
    // }

    // t1_old = t1;
    // t2_old = t2;

    // d_time = timer*0.00001;

    if (vio->cam_id == MT9V034_ID)
    {
        if (vio->pixformat == PIXFORMAT_GRAYSCALE)
        {
            myImage = QImage(vio->img.img[index], vio->img.width, vio->img.high, QImage::Format_Grayscale8);

            if (showFlag == Qt::CheckState::Unchecked)
            {
                emit visionImageSignals(QPixmap::fromImage(myImage));
            }

            cvProcess(myImage, vio->img.qtime[index]);

            // if (vio->isCapImage)
            // {
            //     // timeNow =  QDateTime::currentDateTime();
            //     // QString current_date = timeNow.toString("yyyy_MM_dd_hh_mm_ss_zzz");
            //     QString file_name = vio->saveImagePath + "/camera" + vio->name + "_" + QString::number(vio->saveCount) + ".png";
            //     pixImage.save(file_name);
            //     //mlog->show(file_name);
            //     vio->saveCount++;
            //     vio->isCapImage = false;
            // }
        }
    }
    else
    {
        return;
    }

    //QImage myImage = QImage(vio->img.img,vio->img.width,vio->img.high,QImage::Format_);
}

void CamProcess::cvProcess(QImage qImage, QDateTime time)
{
    Mat image;
    Mat sourceImg = cv::Mat(qImage.height(), qImage.width(), CV_8UC1, qImage.bits());
    threshold(sourceImg, image, 200, 255.0, THRESH_BINARY);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
    Mat imageContours = Mat::zeros(image.size(), CV_8UC1);
    Mat Contours = Mat::zeros(image.size(), CV_8UC1); //绘制

    //DBG("found number %d",contours.size());

    vector<Point2f> centers(contours.size()); //圆心
    vector<float> radius(contours.size());    //半径

    CAMERA_RESULT result;
    result.camIndex = vio->number;
    result.pointNum = (int)0;
    result.time = time.toMSecsSinceEpoch();
    result.path = vio->saveImagePath;
    result.image = qImage;

    if (contours.size() >= 36)
    {
        result.pointNum = 36;
    }
    else
    {
        result.pointNum = contours.size();
    }

    for (int i = 0; i < contours.size(); i++)
    {
        //contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数
        for (int j = 0; j < contours[i].size(); j++)
        {
            //绘制出contours向量内所有的像素点
            Point P = Point(contours[i][j].x, contours[i][j].y);
            Contours.at<uchar>(P) = 255;
        }

        //绘制轮廓
        //drawContours(imageContours,contours,i,Scalar(255),1,1,hierarchy);

        //寻找并绘制最小圆
        minEnclosingCircle(contours[i], centers[i], radius[i]);
        circle(imageContours, centers[i], radius[i], Scalar(255), 1);

        if (showFlag == Qt::CheckState::PartiallyChecked)
        {
            drawMarker(sourceImg, centers[i], Scalar(255, 0, 0), MarkerTypes::MARKER_CROSS, 20, 1, 8);
        }

        if (i < PT_NUM_MAX)
        {
            result.x[i] = centers[i].x;
            result.y[i] = centers[i].y;
        }
    }

    emit positionSignals(result);

    cv::cvtColor(imageContours, image, cv::COLOR_BGR2RGB);
    QImage qImg = QImage((const unsigned char *)(image.data), image.cols, image.rows, image.step, QImage::Format_RGB888);

    if (showFlag == Qt::CheckState::PartiallyChecked)
    {
        cv::cvtColor(sourceImg, sourceImg, cv::COLOR_BGR2RGB);
        QImage qSourceImg = QImage((const unsigned char *)(sourceImg.data), sourceImg.cols, sourceImg.rows, sourceImg.step, QImage::Format_RGB888);
        emit visionImageSignals(QPixmap::fromImage(qSourceImg));
    }

    else if (showFlag == Qt::CheckState::Checked)
        emit visionImageSignals(QPixmap::fromImage(qImg));
}
