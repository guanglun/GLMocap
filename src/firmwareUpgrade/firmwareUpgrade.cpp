#include "firmwareUpgrade.h"

FirmwareUpgrade::FirmwareUpgrade(OPENVIO *vio)
{
    this->vio = vio;
}

void FirmwareUpgrade::upgradeStart()
{
    unsigned char bugger[1024]="hello";
    //for(int i=0;i<1000;i++)
        vio->sendBulk((unsigned char *)bugger,12);
}

