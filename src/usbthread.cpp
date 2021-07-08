#include "usbthread.h"

USBThread::USBThread()
{
    is_loop = true;
}

void USBThread::init(WinUSBDriver *qwindriver,QString buikName)
{
    this->qwindriver = qwindriver;
    this->buikName = buikName;
}

void USBThread::run()
{
    is_loop = true;
    qDebug() << buikName << " usb thread start";
    
    if(!buikName.compare(QString::fromUtf8("cam")))
    {
        qwindriver->CamRecv();
        DBG("qwindriver->CamRecv();");
    }else if(!buikName.compare(QString::fromUtf8("imu"))){
        qwindriver->IMURecv();
        DBG("qwindriver->IMURecv();");
    }

    
    qDebug() << buikName << " usb thread exit";
    is_loop = false;
}

void USBThread::waitClose()
{
    while(is_loop)
    {
        msleep(10);
    }
}


