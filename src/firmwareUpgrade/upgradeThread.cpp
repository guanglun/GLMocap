#include "upgradeThread.h"

#define IAP_START_WAIT  10000
#define IAP_TRANS_WAIT  5000
#define IAP_CRC_WAIT    1000

UpgradeThread::UpgradeThread(FirmwareUpgrade *firmwareUpgrade)
{
    is_loop = true;
    this->firmwareUpgrade = firmwareUpgrade;
    
}
void UpgradeThread::setBinPath(QString binPath)
{
    this->binPath = binPath;
}

void UpgradeThread::run()
{
    
    unsigned int size;
    unsigned int crc = 0;
    int transNum;
    QString str;
    REPLY ret;

    int step = (FRAME_DATA_MAX_SIZE/32*32);

    mlog->show("IAP Thread Start");
    mlog->show("firmware path : " + binPath);
    mlog->show("start upgrade");

    QFile binFile(binPath);
    if(!binFile.open(QIODevice::ReadOnly)){
        mlog->show("firmware open fail");
        goto exit;
    }

    //IAP BEGIN
    size = binFile.size();

    str.sprintf("firmeare size : %d",size);
    mlog->show(str);

    firmwareUpgrade->send_iap_begin(size);

    ret = waitReply(IAP_START_WAIT);
    if(ret == REPLY_WAIT)
    {
        mlog->show("IAP START Timeout");
        goto exit;
    }else if(ret == REPLY_FAIL)
    {
        str.sprintf("IAP START Fail ERRORCODE : %d",firmwareUpgrade->errorCode);
        mlog->show(str);
        goto exit;
    }else{
        mlog->show("IAP START Success");
    }

    //IAP TRANS
    transNum = size/step;
    if(size % step > 0)
    {
        transNum += 1;
    }

    for(int tarnsCnt = 0;tarnsCnt < transNum;tarnsCnt++)
    {
        QByteArray arry = binFile.read(step);

        firmwareUpgrade->send_iap_trans(tarnsCnt,(unsigned char *)arry.data(),arry.length());

        ret = waitReply(IAP_CRC_WAIT);
        if(ret == REPLY_WAIT)
        {
            str.sprintf("IAP TRANS %d timeout",tarnsCnt);
            mlog->show(str);
            goto exit;
        }else if(ret == REPLY_FAIL)
        {
            str.sprintf("IAP TRANS %d Fail ERRORCODE : %d",tarnsCnt,firmwareUpgrade->errorCode);
            mlog->show(str);
            goto exit;
        }else{
            str.sprintf("IAP TRANS %d %d%%",tarnsCnt,(tarnsCnt+1)*100/transNum);
            mlog->show(str);
            //emit iapStatusSignals((tarnsCnt+1)*100/transNum,str);
        }

        for(int cnt = 0;cnt<arry.length();cnt++)
        {
            crc += (unsigned char)arry[cnt];
        }
        crc = ~crc;
    }

    //CRC
    firmwareUpgrade->send_iap_crc(crc);
    ret = waitReply(IAP_CRC_WAIT);
    if(ret == REPLY_WAIT)
    {
        mlog->show("IAP CRC Timeout");
        goto exit;
    }else if(ret == REPLY_FAIL)
    {
        str.sprintf("IAP CRC Fail ERRORCODE : %d",firmwareUpgrade->errorCode);
        mlog->show(str);
        goto exit;
    }else{
        mlog->show("IAP CRC Success");
    }

    //reboot
    firmwareUpgrade->send_iap_reboot();
    mlog->show("IAP SUCCESS");

    exit:
    mlog->show("IAP Thread Exit");
    emit endSignals();

}

void UpgradeThread::waitClose()
{
    while(is_loop)
    {
        msleep(10);
    }
}

enum REPLY UpgradeThread::waitReply(int wait_delay)
{
    int delay_cnt = wait_delay/10;
    firmwareUpgrade->reply_status = REPLY_WAIT;

    while(delay_cnt--)
    {
        if(firmwareUpgrade->reply_status == REPLY_OK)
        {
            return REPLY_OK;
        }else if(firmwareUpgrade->reply_status == REPLY_FAIL)
        {
            return REPLY_FAIL;
        }

        msleep(10);
    }

    return REPLY_WAIT;
}


