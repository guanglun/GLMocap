#include "firmwareUpgrade.h"

FirmwareUpgrade::FirmwareUpgrade(OPENVIO *vio)
{
    this->vio = vio;
}

void FirmwareUpgrade::upgradeStart()
{
    vio->sendBulk((unsigned char *)"hello openvio",strlen("hello openvio"));
}

