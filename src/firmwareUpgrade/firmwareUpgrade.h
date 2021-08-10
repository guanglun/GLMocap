#ifndef FIRMWAREUPGRADE_H
#define FIRMWAREUPGRADE_H

#include "workspace.h"
#include <QObject>
#include <QApplication>
#include <QThread>

class OPENVIO;
#include "openvio.h"

#include "protocol.h"

class UpgradeRecvThread;
#include "upgradeRecvThread.h"

class UpgradeThread;
#include "upgradeThread.h"

#define CMD_IAP_BEGIN 0xA1  //IAP开启指令
#define CMD_IAP_TRANS 0xA2  //IAP写固件指令
#define CMD_IAP_VERIFY 0xA3 //IAP校验固件指令
#define CMD_IAP_RESET 0xA4  //系统复位指令
#define CMD_IAP_ACK 0xA5    //固件下载应答帧
#define CMD_IAP_READY 0xA6  //固件下载准备

typedef enum
{
    IAP_OK = 0x00,
    IAPERROR_SIZE = 0x01,
    IAPERROR_ERASE = 0x02,
    IAPERROR_WRITEFLASH = 0x03,
    IAPERROR_UNLOCK = 0x04,
    IAPERROR_INDEX = 0x05,
    IAPERROR_BUSY = 0x06,
    IAPERROR_FORM = 0x07,
    IAPERROR_CRC = 0x08,
    IAPERROR_OTHER = 0x09,
    IAPERROR_HEAD = 0x10,
} IAP_STATUS;


class FirmwareUpgrade : public QObject
{
    Q_OBJECT
private:
    OPENVIO *vio;
    UpgradeRecvThread *upgradeRecvThread;
    UpgradeThread *upgradeThread;
    FRAME_STRUCT frame_s_tmp;

public:
    enum REPLY reply_status;
    int errorCode;

    FirmwareUpgrade(OPENVIO *vio);
    void setBinPath(QString binPath);
    void upgradeStart();
    void send_iap_begin(unsigned int size);
    void send_iap_trans(unsigned short index,unsigned char *buffer,unsigned int len);
    void send_iap_crc(unsigned int crc);
    void send_iap_reboot();
signals:
    //void camSignals(int index);
    //void imuSignals(int index);
    //void closeSignals(void);

private slots:
    void recvSlot(unsigned char *buffer, int len);
    void endSlot(void);
};

#endif
