#include "VisionProcess.h"


VisionProcess::VisionProcess(QObject* parent)
{

}

void VisionProcess::positionSlot(CAMERA_RESULT result)
{
    
    QString current_date = result.time.toString("yyyy_MM_dd_hh_mm_ss_zzz");
    //mlog->show(current_date);
    DBG("recv %d %s %lf %lf",result.camIndex,(unsigned char *)current_date.toLatin1().data(),result.x,result.y);
}
