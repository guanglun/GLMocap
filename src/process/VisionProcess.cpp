#include "VisionProcess.h"


VisionProcess::VisionProcess(QObject* parent)
{

}

void VisionProcess::init(void)
{

}

void VisionProcess::positionSlot(CAMERA_RESULT result)
{
    camResult[result.camIndex] = result;

mlog->show(QString::number(result.camIndex) +  + " " + QString::number(result.time));

    if( qAbs(camResult[0].time-camResult[1].time) < 10 &&
        qAbs(camResult[0].time-camResult[2].time) < 10 &&
        qAbs(camResult[0].time-camResult[3].time) < 10)
    {

        mlog->show("====>>>" + QString::number(camResult[0].time) + " " + QString::number(camResult[0].time - lastTime));

        lastTime = camResult[0].time;
        
    }
}
