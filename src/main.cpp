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

    // Mat image = imread("D:\\3.code\\MatlabLIB\\image5\\camera1\\camera1_0.png", IMREAD_GRAYSCALE);
    // Mat temp;
    // QDateTime t1,t2;
    // t1 = QDateTime::currentDateTime();
    // threshold(image, image, 150, 255.0, THRESH_BINARY);
    // //QThread::msleep(100);
    // t2 = QDateTime::currentDateTime();

    // qDebug().noquote() << QString::number(t2.toMSecsSinceEpoch()-t1.toMSecsSinceEpoch());

    return a.exec();
}
