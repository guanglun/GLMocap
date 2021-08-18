#include "MuItemCtrlThread.h"

MuItemCtrlThread::MuItemCtrlThread(MuItemCtrl *muItemCtrl)
{
    this->muItemCtrl = muItemCtrl;
}

void MuItemCtrlThread::run()
{
    OPENVIO *vio;
    int i;

    mlog->show("MuItemCtrlThread start");
    idlist.clear();
    mlog->show("All Device:");

    for (QMap<uint8_t, OPENVIO *>::Iterator it = muItemCtrl->vioMap->begin();
         it != muItemCtrl->vioMap->end(); it++)
    {
        mlog->show(QString(it.value()->idShort));
        it.value()->setStatus("WAIT Upgrade");
        idlist.append(QString(it.value()->idStr));
    }

    for (i = 0; i < idlist.size(); i++)
    {
        vio = nullptr;
        for (QMap<uint8_t, OPENVIO *>::Iterator it = muItemCtrl->vioMap->begin();
             it != muItemCtrl->vioMap->end(); it++)
        {
            if (idlist.at(i).contains(QString(it.value()->idStr)))
            {
                vio = it.value();
                break;
            }
        }

        if (vio != nullptr)
        {
            if (muItemCtrl->type == CTRL_TYPE_UPGRADE)
            {
                upgrade(vio);
            }
        }
    }

    mlog->show("MuItemCtrlThread exit");
}

int MuItemCtrlThread::upgrade(OPENVIO *vio)
{
    int waitCount = 3000;
    mlog->show("upgrade " + QString(vio->idShort) + " start");

    muItemCtrl->firmwareUpgrade->setOPENVIO(vio);
    muItemCtrl->firmwareUpgrade->setBinPath(muItemCtrl->binPath);
    muItemCtrl->firmwareUpgrade->upgradeStart();

    while (muItemCtrl->firmwareUpgrade->state != FirmwareUpgrade::STATE_SUCCESS && waitCount > 0)
    {
        msleep(10);
        waitCount--;
    }

    if (waitCount <= 0)
    {
        mlog->show("upgrade " + QString(vio->idShort) + " timeout");
    }
    mlog->show("upgrade " + QString(vio->idShort) + " exit");

    return 0;
}
