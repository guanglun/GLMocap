#include "usbthread.h"

USBThread::USBThread()
{
    is_loop = true;
}

void USBThread::init(OPENVIO *vio,QString buikName)
{
    this->vio = vio;
    this->buikName = buikName;
}

void USBThread::run()
{
    is_loop = true;
    //qDebug() << buikName << " usb thread start";
    
    if(!buikName.compare(QString::fromUtf8("cam")))
    {
        vio->CamRecv();
        //DBG("qwindriver->CamRecv();");
    }else if(!buikName.compare(QString::fromUtf8("imu"))){
        vio->IMURecv();
        //DBG("qwindriver->IMURecv();");
    }

    
    //qDebug() << buikName << " usb thread exit";
    is_loop = false;
}

void USBThread::waitClose()
{
    while(is_loop)
    {
        msleep(10);
    }
}


