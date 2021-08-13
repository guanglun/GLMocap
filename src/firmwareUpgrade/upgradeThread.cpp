#include "upgradeThread.h"

#define IAP_START_WAIT  10000
#define IAP_TRANS_WAIT  5000
#define IAP_CRC_WAIT    5000

UpgradeThread::UpgradeThread(FirmwareUpgrade *firmwareUpgrade)
{
    is_loop = true;
    this->firmwareUpgrade = firmwareUpgrade;
    wait_id = nullptr;
}

void UpgradeThread::setBinPath(QString binPath)
{
    this->binPath = binPath;
}

void UpgradeThread::newSlot(OPENVIO *vio)
{
    if (wait_id != nullptr)
    {
        if (QString::compare(QString(vio->idStr), wait_id) == 0 && vio->type == wait_type)
        {
            DBG("the wait id come in");
            if(wait_type == TYPE_BOOTLOADER)
                firmwareUpgrade->setOPENVIO(vio);
            else if(wait_type == TYPE_OPENVIO)
                firmwareUpgrade->setOPENVIO(vio,false);
            wait_id = nullptr;
        }
    }
}

void UpgradeThread::showStatus(QString str)
{
    mlog->show(str);
    if (firmwareUpgrade->vio != nullptr)
        firmwareUpgrade->vio->setStatus(str);
}

void UpgradeThread::run()
{

    unsigned int size;
    unsigned int crc = 0;
    int transNum;
    QString str;
    REPLY ret;
    int waitCount = 500;

    wait_id = nullptr;

    int step = (FRAME_DATA_MAX_SIZE / 32 * 32);

    showStatus("IAP Thread Start");
    showStatus("firmware path : " + binPath);
    showStatus("start upgrade");

    QFile binFile(binPath);
    if (!binFile.open(QIODevice::ReadOnly))
    {
        showStatus("firmware open fail");
        goto exit;
    }

    if (firmwareUpgrade->vio->type == TYPE_OPENVIO)
    {
        wait_id = QString(firmwareUpgrade->vio->idStr);

        showStatus("WAIT Bootloader ...");

        firmwareUpgrade->send_iap_reboot_to_bootloader();
        firmwareUpgrade->vio = nullptr;
        
        wait_type = TYPE_BOOTLOADER;

        while (waitCount--)
        {
            if (wait_id == nullptr)
            {
                break;
            }
            msleep(10);
        }

        if (waitCount <= 0)
        {
            showStatus("WAIT Bootloader Timeout");
            goto exit;
        }
    }

    firmwareUpgrade->upgradeRecvThread = new UpgradeRecvThread(firmwareUpgrade->vio);
    connect(firmwareUpgrade->upgradeRecvThread, SIGNAL(recvSignals(unsigned char *, int)), firmwareUpgrade, SLOT(recvSlot(unsigned char *, int)));
    firmwareUpgrade->upgradeRecvThread->start();

    //IAP BEGIN
    size = binFile.size();

    str.sprintf("firmeare size : %d", size);
    showStatus(str);

    msleep(100);
    firmwareUpgrade->send_iap_begin(size);

    showStatus("IAP START ERASE ");

    ret = waitReply(IAP_START_WAIT);
    if (ret == REPLY_WAIT)
    {
        showStatus("IAP START Timeout");
        goto exit;
    }
    else if (ret == REPLY_FAIL)
    {
        str.sprintf("IAP START Fail ERRORCODE : %d", firmwareUpgrade->errorCode);
        showStatus(str);
        goto exit;
    }
    else
    {
        showStatus("IAP START Success");
    }

    //IAP TRANS
    transNum = size / step;
    if (size % step > 0)
    {
        transNum += 1;
    }

    for (int tarnsCnt = 0; tarnsCnt < transNum; tarnsCnt++)
    {
        QByteArray arry = binFile.read(step);

        firmwareUpgrade->send_iap_trans(tarnsCnt, (unsigned char *)arry.data(), arry.length());

        ret = waitReply(IAP_TRANS_WAIT);
        if (ret == REPLY_WAIT)
        {
            str.sprintf("IAP TRANS %d timeout", tarnsCnt);
            showStatus(str);
            goto exit;
        }
        else if (ret == REPLY_FAIL)
        {
            str.sprintf("IAP TRANS %d Fail ERRORCODE : %d", tarnsCnt, firmwareUpgrade->errorCode);
            showStatus(str);
            goto exit;
        }
        else
        {
            str.sprintf("IAP TRANS %d %d%%", tarnsCnt, (tarnsCnt + 1) * 100 / transNum);
            showStatus(str);
            //emit iapStatusSignals((tarnsCnt+1)*100/transNum,str);
        }

        for (int cnt = 0; cnt < arry.length(); cnt++)
        {
            crc += (unsigned char)arry[cnt];
        }
        crc = ~crc;
    }

    //CRC
    firmwareUpgrade->send_iap_crc(crc);
    ret = waitReply(IAP_CRC_WAIT);
    if (ret == REPLY_WAIT)
    {
        showStatus("IAP CRC Timeout");
        goto exit;
    }
    else if (ret == REPLY_FAIL)
    {
        str.sprintf("IAP CRC Fail ERRORCODE : %d", firmwareUpgrade->errorCode);
        showStatus(str);
        goto exit;
    }
    else
    {
        showStatus("IAP CRC Success");
    }

    wait_id = QString(firmwareUpgrade->vio->idStr);

    showStatus("WAIT Reboot ...");
    //reboot
    firmwareUpgrade->send_iap_reboot();
    firmwareUpgrade->vio = nullptr;

    waitCount = 500;
    wait_type = TYPE_OPENVIO;

    while (waitCount--)
    {
        if (wait_id == nullptr)
        {
            break;
        }
        msleep(10);
    }

    if (waitCount <= 0)
    {
        showStatus("WAIT Reboot Timeout");
        goto exit;
    }

    showStatus("IAP SUCCESS");
    firmwareUpgrade->vio->close();
exit:
    wait_id = nullptr;
    mlog->show("IAP Thread Exit");
    emit endSignals();
}

void UpgradeThread::waitClose()
{
    while (is_loop)
    {
        msleep(10);
    }
}

enum REPLY UpgradeThread::waitReply(int wait_delay)
{
    int delay_cnt = wait_delay / 10;
    firmwareUpgrade->reply_status = REPLY_WAIT;

    while (delay_cnt--)
    {
        if (firmwareUpgrade->reply_status == REPLY_OK)
        {
            return REPLY_OK;
        }
        else if (firmwareUpgrade->reply_status == REPLY_FAIL)
        {
            return REPLY_FAIL;
        }

        msleep(10);
    }

    return REPLY_WAIT;
}
