#ifndef UPGRADETHREAD_H
#define UPGRADETHREAD_H

#include <QThread>
#include "libusb.h"
#include "workspace.h"
#include <QList>

enum REPLY
{
    REPLY_WAIT,
    REPLY_OK,
    REPLY_FAIL
};

class firmwareUpgrade;
#include "firmwareUpgrade.h"

class WinUSBDriver;
#include "winusbdriver.h"


class UpgradeThread : public QThread
{
    Q_OBJECT
private:

    enum REPLY waitReply(int wait_delay);

    FirmwareUpgrade *firmwareUpgrade;
    bool is_loop;
    QString binPath;
public:
    QString wait_id;
    enum OPENVIO_TYPE wait_type;
    
    UpgradeThread(FirmwareUpgrade *firmwareUpgrade);
    void run();
    void waitClose();
    void showStatus(QString str);
    void setBinPath(QString binPath);
signals:
    void endSignals(void);
    // void imuSignals(int index);   
    // void closeSignals(void);   
private slots:
    void newSlot(OPENVIO *vio);
    //void closeSlot(void);    
};

#endif // USBTHREAD_H
