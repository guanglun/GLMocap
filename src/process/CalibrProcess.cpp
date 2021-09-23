#include "CalibrProcess.h"

CalibrProcess::CalibrProcess(QObject *parent)
{
}

void CalibrProcess::startSlot(void)
{
    int pc = 0;
    mlog->show("calibr process start");
    if (isRuning)
    {
        mlog->show("error,calibr process running already");
        goto end;
    }
    isRuning = true;


    calibr.calibrStart();


end:
    isRuning = false;
    isStop = false;
    mlog->show("calibr process end");
}

void CalibrProcess::stop(void)
{
    mlog->show("stop CalibrProcess slot");
    if (isRuning)
        isStop = true;
}