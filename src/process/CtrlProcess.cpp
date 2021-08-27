#include "CtrlProcess.h"

CtrlProcess::CtrlProcess(QObject *parent)
{
}

void CtrlProcess::setVio(QMap<uint8_t, OPENVIO*> *vioMap,OPENVIO *setvio)
{
    this->vioMap = vioMap;
    this->setvio = setvio;
}

OPENVIO *CtrlProcess::getEach()
{
    OPENVIO *vio = NULL;
    
    if (setvio != NULL && vioMap == NULL && count == 0)
    {
        vio = setvio;
    }
    else if (setvio == NULL && vioMap != NULL && count == 0)
    {
        
        it = vioMap->begin();
        if(it != vioMap->end())
            vio = it.value();
    }else if (setvio == NULL && vioMap != NULL)
    {
        it++;
        if(it != vioMap->end())
            vio = it.value();
    }
    count++;
    return vio;
}

int CtrlProcess::getLen(void)
{
    OPENVIO *vio = NULL;
    if (vio != NULL && vioMap == NULL)
    {
        return 1;
    }
    else if (vio == NULL && vioMap != NULL)
    {
        return vioMap->size();
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
                mlog->show("Set Exposure " + QString::number(exposure) + " Success");
                vio->setStatus("Set Exposure " + QString::number(exposure) + " Success");
            }
            else
            {
                mlog->show("Set Exposure " + QString::number(exposure) + " Fail");
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
                mlog->show("Set CamStatus " + QString::number(state) + " Success");
                vio->setStatus("Set CamStatus " + QString::number(state) + " Success");
            }
            else
            {
                mlog->show("Set CamStatus " + QString::number(state) + " Fail");
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
                mlog->show("Set SyncStatus " + QString::number(state) + " Success");
                vio->setStatus("Set SyncStatus " + QString::number(state) + " Success");
            }
            else
            {
                mlog->show("Set SyncStatus " + QString::number(state) + " Fail");
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
                mlog->show("Set SyncMode " + QString::number(mode) + " Success");
                vio->setStatus("Set SyncMode " + QString::number(mode) + " Success");
            }
            else
            {
                mlog->show("Set SyncMode " + QString::number(mode) + " Fail");
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
                mlog->show("Set fps " + QString::number(fps) + " Success");
                vio->setStatus("Set fps " + QString::number(fps) + " Success");
            }
            else
            {
                mlog->show("Set fps " + QString::number(fps) + " Fail");
                vio->setStatus("Set fps " + QString::number(fps) + " Fail");
            }
        }
    } while (vio != NULL);
}

void CtrlProcess::ctrlInfraredPwmSlot(unsigned char pwm)
{
    OPENVIO *vio = NULL;
    count = 0;
    do
    {
        vio = getEach();
        if (vio != NULL)
        {
            vio->open();
            if(vio->ctrlInfraredPwm(pwm) == 0)
            {
                mlog->show("Set InfraredPwm " + QString::number(pwm) + " Success");
                vio->setStatus("Set InfraredPwm " + QString::number(pwm) + " Success");
            }
            else
            {
                mlog->show("Set InfraredPwm " + QString::number(pwm) + " Fail");
                vio->setStatus("Set InfraredPwm " + QString::number(pwm) + " Fail");
            }
        }
    } while (vio != NULL);
}

void CtrlProcess::ctrlMultemCamStartSlot(void)
{
    ctrlCamStatusSlot(1);
    ctrlCamSyncStatusSlot(1);
}

void CtrlProcess::ctrlMultemCamStopSlot(void)
{
    ctrlCamStatusSlot(0);
    ctrlCamSyncStatusSlot(0);
}