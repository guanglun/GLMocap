#ifndef UPGRADERECVTHREAD_H
#define UPGRADERECVTHREAD_H

#include <QThread>
#include "libusb.h"
#include "workspace.h"


class OPENVIO;
#include "openvio.h"

class WinUSBDriver;
#include "winusbdriver.h"

class UpgradeRecvThread : public QThread
{
    Q_OBJECT
private:
    
    OPENVIO *vio;
    bool is_loop;
    QString buikName;
    unsigned char recvBuffer[512];
    int recvLen;
public:
    UpgradeRecvThread(OPENVIO *vio);
    void run();
    void waitClose();
    
signals:
    void recvSignals(unsigned char *buffer,int len);
    // void imuSignals(int index);   
    // void closeSignals(void);   
private slots:
    //void closeSlot(void);    
};

#endif // USBTHREAD_H
