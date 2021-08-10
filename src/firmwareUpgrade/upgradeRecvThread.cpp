#include "upgradeRecvThread.h"

UpgradeRecvThread::UpgradeRecvThread(OPENVIO *vio)
{
    is_loop = true;
    this->vio = vio;
}

void UpgradeRecvThread::run()
{
    is_loop = true;
    qDebug() << " UpgradeRecvThread start";
    
    while(is_loop)
    {
        recvLen = vio->recvBulk(recvBuffer,512);
        if(recvLen > 0)
        {
            emit recvSignals(recvBuffer,recvLen);
        }else
        {
            is_loop = false;
        }

    }

    qDebug() <<  " UpgradeRecvThread exit";
    is_loop = false;
}

void UpgradeRecvThread::waitClose()
{
    while(is_loop)
    {
        msleep(10);
    }
}


