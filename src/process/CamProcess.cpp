#include "CamProcess.h"

CamProcess::CamProcess(OPENVIO *vio,QObject* parent)
{
    this->vio = vio;
}

void CamProcess::camSlot(int index)
{
    static uint32_t timer;
    static uint32_t t1,t1_old;
    static uint16_t t2,t2_old;
    static bool is_first = true;
    static float d_time = 0;

    QImage myImage;
    QPixmap pixImage;
    QDateTime timeNow;

    t1 = (uint32_t)(vio->img.time[index][0]<<24);
    t1 |= (uint32_t)(vio->img.time[index][1]<<16);
    t1 |= (uint32_t)(vio->img.time[index][2]<<8);
    t1 |= (uint32_t)(vio->img.time[index][3]<<0);

    t2 = (uint16_t)(vio->img.time[index][4]<<8);
    t2 |= (uint16_t)(vio->img.time[index][5]<<0);

    if(is_first == true)
    {
        is_first = false;
        t1_old = t1;
        t2_old = t2;
        return;
    }

    if(t2 > t2_old)
    {
        timer = t2-t2_old;
    }else{
        timer = (uint32_t)t2 + 50000 - t2_old;
    }

    t1_old = t1;
    t2_old = t2;

    d_time = timer*0.00001;
    //DBG("cam %d\t%d\t%d\t%f",t1,t2,timer,d_time);

    if(vio->cam_id == MT9V034_ID)
    {
        if(vio->pixformat == PIXFORMAT_GRAYSCALE)
        {
            myImage = QImage(vio->img.img[index],vio->img.width,vio->img.high,QImage::Format_Grayscale8);
            
            emit visionImageSignals(QPixmap::fromImage(myImage));

            if(vio->isCapImage)
            {
                
                // timeNow =  QDateTime::currentDateTime();
                // QString current_date = timeNow.toString("yyyy_MM_dd_hh_mm_ss_zzz");
                QString file_name = vio->saveImagePath + "/" + vio->name + "_" + QString::number(vio->saveCount) + ".png";
                pixImage.save(file_name);
                //mlog->show(file_name);
                vio->saveCount++;
                vio->isCapImage = false;
            }
        }
    }else{
        return;
    }
    
    //QImage myImage = QImage(vio->img.img,vio->img.width,vio->img.high,QImage::Format_);
    
}