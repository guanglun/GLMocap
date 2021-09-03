#include "CamProcess.h"

CamProcess::CamProcess(OPENVIO *vio, QObject *parent)
{
    this->vio = vio;
    this->hPoint = new QHash<POINT_STATE, GLPoint *>();
}

void CamProcess::setShowFlag(Qt::CheckState flag)
{
    showFlag = flag;
}

double distance(Point2f p1, Point2f p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void CamProcess::match(vector<Point2f> centers)
{
    if (vio->visionProcess->matchState == MATCH_START)
    {
        vPoint.clear();
        for (int i = 0; i < centers.size(); i++)
        {
            vPoint.push_back(new GLPoint(POINT_STATE_NEW_FOUND, vio->number, centers[i].x, centers[i].y));
        }
    }
    else if (vio->visionProcess->matchState == MATCH_OK)
    {
        if (vPoint.size() > 0 && centers.size() > 0)
        {
            for (int i = 0; i < vPoint.size(); i++)
            {
                int minIndex = 0;
                double min = 9999999, minTmp;
                for (int cm = 0; cm < centers.size(); cm++)
                {
                    minTmp = distance(Point2f(vPoint[i]->x, vPoint[i]->y), centers[cm]);
                    if (minTmp < min)
                    {
                        min = minTmp;
                        minIndex = cm;
                    }
                }

                vPoint[i]->x = centers[minIndex].x;
                vPoint[i]->y = centers[minIndex].y;
                //vPoint[i]->setXY(centers[minIndex].x,centers[minIndex].y);
            }
        }
    }

    // if (vio->visionProcess->matchState == MATCH_START ||
    //     vio->visionProcess->matchState == MATCH_OK)
    {
        result.vPoint = vPoint;
        emit positionSignals(result);
    }
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
    this->index = index;
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
    // qint64 t1,t2;
    // t1 = QDateTime::currentDateTime().toMSecsSinceEpoch();

    Mat image;
    Mat sourceImg = cv::Mat(qImage.height(), qImage.width(), CV_8UC1, qImage.bits());

    threshold(sourceImg, image, setting->threshold, 255.0, THRESH_BINARY);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
    Mat imageContours = Mat::zeros(image.size(), CV_8UC1);
    Mat Contours = Mat::zeros(image.size(), CV_8UC1); //绘制

    //DBG("found number %d",contours.size());

    vector<Point2f> points(contours.size());  //位置
    vector<Point2f> centers(contours.size()); //圆心
    vector<float> radius(contours.size());    //半径

    result.camIndex = vio->number;
    result.pointNum = (int)0;
    result.time = time.toMSecsSinceEpoch();
    result.path = vio->saveImagePath;
    result.image = qImage;
    result.hPoint = hPoint;

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
            points[i].x += contours[i][j].x;
            points[i].y += contours[i][j].y;

            Point P = Point(contours[i][j].x, contours[i][j].y);
            Contours.at<uchar>(P) = 255;
        }

        points[i].x /= contours[i].size();
        points[i].y /= contours[i].size();

        //绘制轮廓
        //drawContours(imageContours,contours,i,Scalar(255),1,1,hierarchy);

        //寻找并绘制最小圆
        minEnclosingCircle(contours[i], centers[i], radius[i]);
        circle(imageContours, centers[i], radius[i], Scalar(255), 1);

        if (i < PT_NUM_MAX)
        {
            result.x[i] = points[i].x;
            result.y[i] = points[i].y;
        }
    }

    match(points);

    if (showFlag == Qt::CheckState::PartiallyChecked)
    {
        for (int i = 0; i < vPoint.size(); i++)
        {
            if (vio->visionProcess->matchState == MATCH_OK)
            {
                putText(sourceImg, std::to_string(vPoint[i]->id), Point(vPoint[i]->x, vPoint[i]->y - 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 2);
            }

            drawMarker(sourceImg, Point2f(vPoint[i]->x, vPoint[i]->y), Scalar(255, 0, 0), MarkerTypes::MARKER_CROSS, 20, 1, 8);
        }
    }

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

    // t2 = QDateTime::currentDateTime().toMSecsSinceEpoch();
    // mlog->show(QString::number(vio->number) + " diff " + QString::number(t2-time.toMSecsSinceEpoch()));
}
