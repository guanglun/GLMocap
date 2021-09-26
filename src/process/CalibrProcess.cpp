#include "CalibrProcess.h"

CalibrProcess::CalibrProcess(QObject *parent)
{
}

void CalibrProcess::startSlot(QString path)
{
    int pc = 0;
    


    mlog->show("calibr process start, " + path);
    if (isRuning)
    {
        mlog->show("error,calibr process running already");
        goto end;
    }
    isRuning = true;


    calibr.calibrStart(path);


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