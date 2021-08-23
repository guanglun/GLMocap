#include <QApplication>

#include "formcamwindow.h"
#include "formcvwindow.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    FormCamWindow fCamWindow;
    fCamWindow.show();

    // Mat image = imread("F:\\windows\\Desktop\\CODE\\MatlabLIB\\image6\\camera0\\camera0_0.png", IMREAD_GRAYSCALE);
    // Mat temp;
    // qint64 t1,t2;
    // while(1)
    // {
    // t1 = QDateTime::currentDateTime().toMSecsSinceEpoch();
    // threshold(temp, image, 150, 255.0, THRESH_BINARY);
    // //QThread::msleep(100);
    // t2 = QDateTime::currentDateTime().toMSecsSinceEpoch();

    // qDebug().noquote() << QString::number(t2-t1);
    // }
    return a.exec();
}
