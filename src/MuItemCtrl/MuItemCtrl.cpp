#include "MuItemCtrl.h"


MuItemCtrl::MuItemCtrl(FormCamWindow *formCamWindow)
{
    this->formCamWindow = formCamWindow;
    firmwareUpgrade = new FirmwareUpgrade();
    connect(formCamWindow->qwinusb, SIGNAL(newSignal(OPENVIO *)), firmwareUpgrade->upgradeThread, SLOT(newSlot(OPENVIO *)));
    muItemCtrlThread = new MuItemCtrlThread(this);
}

void MuItemCtrl::setCtrl(enum CTRL_TYPE type,QMap<uint8_t, OPENVIO*> *vioMap)
{
    this->type = type;
    this->vioMap = vioMap;

    if(type == CTRL_TYPE_UPGRADE)
    {
        QString path = setting->getFirmwarePath();
        if (path.length() == 0)
        {
            path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        }
        QString filePath = QFileDialog::getOpenFileName(
            formCamWindow,
            tr("choose a firmware file"),
            path,
            tr("param(*.bin);;"));
        if (filePath.isEmpty())
        {
            QMessageBox::warning(formCamWindow, "Warning!", "Failed to open the firmware!");
        }
        else
        {
            setting->setFirmwarePath(filePath);
            binPath = filePath;
            start();
        }
    }
}

void MuItemCtrl::start()
{
    muItemCtrlThread->start();
}