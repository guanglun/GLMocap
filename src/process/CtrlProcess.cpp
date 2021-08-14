#include "CtrlProcess.h"

CtrlProcess::CtrlProcess(QObject *parent)
{
}

void CtrlProcess::setVio(QList<OPENVIO *> *vioList,OPENVIO *setvio)
{
    this->vioList = vioList;
    this->setvio = setvio;
}

OPENVIO *CtrlProcess::getEach()
{
    OPENVIO *vio = NULL;
    if (vio != NULL && vioList == NULL && count == 0)
    {
        vio = setvio;
    }
    else if (vio == NULL && vioList != NULL)
    {
        if (count < vioList->size())
        {
            vio = vioList->at(count);
        }
    }
    count++;
    return vio;
}

int CtrlProcess::getLen(void)
{
    OPENVIO *vio = NULL;
    if (vio != NULL && vioList == NULL)
    {
        return 1;
    }
    else if (vio == NULL && vioList != NULL)
    {
        return vioList->size();
        
    }
    return -1;
}

void CtrlProcess::setExposureSlot(int exposure)
{
    OPENVIO *vio = NULL;
    count = 0;
    

    do
    {
        vio = getEach();
        if (vio != NULL)
        {
            vio->open();
            if (vio->ctrlCamSetExposure(exposure) == 0)
            {
                vio->setStatus("Set Exposure " + QString::number(exposure) + " Success");
            }
            else
            {
                vio->setStatus("Set Exposure " + QString::number(exposure) + " Fail");
            }
        }

    } while (vio != NULL);
}

void CtrlProcess::syncSlot(void)
{
    OPENVIO *vio = NULL;
    int len = getLen();

    count = 0;
    do
    {
        vio = getEach();
        if (vio != NULL)
        {
            vio->open();
        }
    } while (vio != NULL);

    count = 0;
    do
    {
        vio = getEach();
        if (vio != NULL)
        {
            vio->ctrlCamStatus(1);
        }
    } while (vio != NULL);

}

void CtrlProcess::ctrlCamStatusSlot(unsigned char state)
{
    OPENVIO *vio = NULL;
    count = 0;
    do
    {
        vio = getEach();
        if (vio != NULL)
        {
            vio->open();
            if(vio->ctrlCamStatus(state) == 0)
            {
                vio->setStatus("Set CamStatus " + QString::number(state) + " Success");
            }
            else
            {
                vio->setStatus("Set CamStatus " + QString::number(state) + " Fail");
            }
        }
    } while (vio != NULL);
}

void CtrlProcess::ctrlCamSyncStatusSlot(unsigned char state)
{
    OPENVIO *vio = NULL;
    count = 0;
    do
    {
        vio = getEach();
        if (vio != NULL)
        {
            vio->open();
            if(vio->ctrlCamSyncStatus(state) == 0)
            {
                vio->setStatus("Set SyncStatus " + QString::number(state) + " Success");
            }
            else
            {
                vio->setStatus("Set SyncStatus " + QString::number(state) + " Fail");
            }
        }
    } while (vio != NULL);
}

void CtrlProcess::ctrlCamSyncModeSlot(unsigned char mode)
{
    OPENVIO *vio = NULL;
    count = 0;
    do
    {
        vio = getEach();
        if (vio != NULL)
        {
            vio->open();
            if(vio->ctrlCamSyncMode(mode) == 0)
            {
                vio->setStatus("Set SyncMode " + QString::number(mode) + " Success");
            }
            else
            {
                vio->setStatus("Set SyncMode " + QString::number(mode) + " Fail");
            }
        }
    } while (vio != NULL);
}

void CtrlProcess::ctrlCamFpsSlot(unsigned char fps)
{
    OPENVIO *vio = NULL;
    count = 0;
    do
    {
        vio = getEach();
        if (vio != NULL)
        {
            vio->open();
            if(vio->ctrlCamFps(fps) == 0)
            {
                vio->setStatus("Set fps " + QString::number(fps) + " Success");
            }
            else
            {
                vio->setStatus("Set fps " + QString::number(fps) + " Fail");
            }
        }
    } while (vio != NULL);
}