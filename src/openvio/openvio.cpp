#include "openvio.h"

OPENVIO::OPENVIO(libusb_device *dev)
{
    this->dev = dev;
    pItem = new QStandardItem();

    camThread = new USBThread();
    imuThread = new USBThread();

    camThread->init(this, "cam");
    imuThread->init(this, "imu");

    camStatus = SENSOR_STATUS_STOP;
    imuStatus = SENSOR_STATUS_STOP;

    connect(this, SIGNAL(closeSignals()), this, SLOT(closeSlot()));
    connect(this, SIGNAL(setStatusSignal(QString)), this, SLOT(setStatusSlot(QString)));
    connect(this, SIGNAL(setSpeedSignal(QString)), this, SLOT(setSpeedSlot(QString)));

    camProcess = new CamProcess(this);
    camProcess->moveToThread(&camProcessThread);
    connect(this, SIGNAL(camSignals(int)), camProcess, SLOT(camSlot(int)));
    camProcessThread.start();
}

OPENVIO::~OPENVIO()
{
    camProcessThread.quit();
    camProcessThread.wait();
}

int OPENVIO::open(void)
{
    int ret = 0;

    if (dev_handle != NULL)
    {
        DBG("already %s open", idShort);
        return 0;
    }

    ret = libusb_open(dev, &dev_handle);
    if (ret < 0)
    {
        DBG("device open fail %d ", ret);
        goto open_fail;
    }
    else
    {
        //DBG("device open success");
    }

    ret = libusb_claim_interface(dev_handle, 0);
    if (ret < 0)
    {
        DBG("claim interface fail %d ", ret);
        goto claim_fail;
    }
    else
    {
        //DBG("claim interface success");
    }
    DBG("open success");

    return 0;

claim_fail:
    libusb_close(dev_handle);
open_fail:
    dev_handle = NULL;
init_fail:
    DBG("open fail");
    return -1;
}

int OPENVIO::close(void)
{
    if (dev_handle != NULL)
    {
        libusb_release_interface(dev_handle, 0);
        libusb_close(dev_handle);
    }
    DBG("close");
    dev_handle = NULL;
    return 0;
}

void OPENVIO::removeReady(void)
{
    if(formVioWindow->isActiveWindow() == true)
    {
        formVioWindow->close();
    }
    close();
    removeItem();
}

void OPENVIO::CamRecv(void)
{
    DBG("cam recv start");
    int ret = 0;
    int img_index = 0;
    int recv_head_status = 0;
    int findRet = 0;
    FindStr findStr;
    findStr.config((unsigned char *)"CAMERA", 6);
    while (isCamRecv)
    {
        try
        {
            ret = libusb_bulk_transfer(dev_handle, CAM_EPADDR, (unsigned char *)(img.img[img_index] + recv_index), 512 * 1024, &camRecvLen, 1000);
        }
        catch (...)
        {
            break;
        }

        if (ret < 0)
        {
            if (ret != -7 && ret != -9)
            {
                //DBG("%s cam recv error %d", name.toStdString().c_str(), ret);
                //emit disconnectSignals();
                break;
            }
            else
            {
                //DBG("cam recv time out");
            }
        }
        else if (camStatus != SENSOR_STATUS_STOP)
        {
            //DBG("cam recv %d %d %d",recv_head_status,camRecvLen,recv_index);
            recv_count_1s += camRecvLen;

            if ((recv_head_status == 0) && (camRecvLen == 12))
            {
                findRet = findStr.input(img.img[img_index], camRecvLen);
                if (findRet > 0)
                {
                    img.qtime[img_index] = QDateTime::currentDateTime();
                    recv_head_status = 1;
                    memcpy(img.time[img_index], img.img[img_index] + 6, 6);
                }
            }
            else if (camRecvLen == 12)
            {
                findRet = findStr.input(img.img[img_index] + recv_index, camRecvLen);
                if (findRet > 0)
                {
                    img.qtime[img_index] = QDateTime::currentDateTime();
                    recv_head_status = 1;
                    memcpy(img.time[img_index], img.img[img_index] + recv_index + 6, 6);
                    recv_index = 0;
                    recv_head_status = 1;
                }
                else
                {
                    recv_index = 0;
                    recv_head_status = 0;
                }
            }
            else if (recv_head_status == 0)
            {
                //DBG("%s cam recv error len %d", name.toStdString().c_str(), camRecvLen);
                //                emit disconnectSignals();
                //                break;
            }
            else
            {
                recv_index += camRecvLen;
                if (recv_index >= (img.size))
                {

                    frame_fps++;
                    recv_index = 0;
                    recv_head_status = 0;

                    //DBG("recv %d", img_index);

                    emit camSignals(img_index);

                    img_index++;
                    if (img_index >= IMG_FRAME_SIZE_MAX)
                    {
                        img_index = 0;
                    }
                }
            }
        }
    }
    DBG("cam recv exit");
}

void OPENVIO::IMURecv(void)
{
    DBG("imu recv start");
    int ret = 0;
    FindStr findStr;
    findStr.config((unsigned char *)"ICMIMU", 6);

    int imu_index = 0;

    while (isIMURecv)
    {
        ret = libusb_bulk_transfer(dev_handle, IMU_EPADDR, (unsigned char *)(img.imu[imu_index]), IMU_PACKAGE_SIZE, &imuRecvLen, 10);
        if (ret < 0)
        {

            if (ret != -7)
            {
                DBG("imu recv error %d", ret);
                //emit disconnectSignals();
                //break;
            }
            else
            {
                //DBG("imu recv time out");
            }
        }
        else
        {
            recv_count_1s += imuRecvLen;

            if (imuRecvLen == IMU_PACKAGE_SIZE)
            {
                //emit imuSignals(imu_index);
                imu_index++;
                if (imu_index >= IMU_FRAME_SIZE_MAX)
                {
                    imu_index = 0;
                }
            }
        }
    }
    DBG("imu recv exit");
}

bool OPENVIO::sendBulk(unsigned char *buffer, int len)
{
    int recvLen = 0;
    int ret = 0;

    ret = libusb_bulk_transfer(dev_handle, CTRL_EPADDR, buffer, len, &recvLen, 0);

    if (ret < 0)
    {
        DBG("sendBulk fail %d", ret);
        return false;
    }
    else
    {
        //DBG("sendBulk success. ret:%d", ret);
        return true;
    }
}

int OPENVIO::recvBulk(unsigned char *buffer, int len)
{
    int recvLen = 0;
    int ret = 0;

    ret = libusb_bulk_transfer(dev_handle, CAM_EPADDR, buffer, len, &recvLen, 0);

    if (ret < 0 && ret != -7)
    {
        DBG("recvBulk fail :%d", ret);
        return -1;
    }
    else
    {
        //DBG("recvBulk success. ret:%d len:%d", ret,recvLen);

        return recvLen;
    }
}

void OPENVIO::setItem(QStandardItemModel *pModelOpenvio)
{
    this->pModelOpenvio = pModelOpenvio;
    number = setting->getNumberById(idStr);
    if (number > -1)
    {
        name = "camera" + QString::number(number);
    }
    else
    {
        name = "";
    }

    itemCamData.id = &idStr[16];
    itemCamData.ver = QString("v" +
                              QString::number(version[0]) + "." +
                              QString::number(version[1]) + "." +
                              QString::number(version[2]));
    itemCamData.status = "wait";
    itemCamData.name = name;
    itemCamData.type = type;
    itemCamData.speed = "";
    pItem->setData(QVariant::fromValue(itemCamData), Qt::UserRole + 1);

    pModelOpenvio->appendRow(pItem);

    // if(number == -1 || pModelOpenvio->rowCount() < number)
    // {
    //     pModelOpenvio->appendRow(pItem);
    // }else{
    //     pModelOpenvio->insertRow(number,pItem);
    // }
}

void OPENVIO::removeItem(void)
{
    QModelIndex index = pModelOpenvio->indexFromItem(pItem);
    pModelOpenvio->removeRow(index.row());
}

int OPENVIO::getRow(void)
{
    return pModelOpenvio->indexFromItem(pItem).row();
}

void OPENVIO::setStatusSlot(QString str)
{
    itemCamData.status = str;
    pItem->setData(QVariant::fromValue(itemCamData));
}

void OPENVIO::setStatus(QString status)
{
    emit setStatusSignal(status);
}

void OPENVIO::setSpeedSlot(QString speed)
{
    itemCamData.speed = speed;
    pItem->setData(QVariant::fromValue(itemCamData));
}

void OPENVIO::setSpeed(QString speed)
{
    emit setSpeedSignal(speed);
}

void OPENVIO::setNumber(int number)
{
    this->number = number;
    if (number > -1)
    {
        name = "camera" + QString::number(number);
    }
    else
    {
        name = "";
    }
    itemCamData.name = name;
    pItem->setData(QVariant::fromValue(itemCamData));
    setting->setNumberById(idStr, number);
}

// int OPENVIO::close(void)
// {
//     emit closeSignals();
// }

void OPENVIO::closeSlot(void)
{
}

int OPENVIO::sendCtrl(char request, uint8_t type, unsigned char *buffer, uint16_t len)
{
    int ret = 0;
    if (dev_handle == NULL)
    {
        open();
    }

    if (dev_handle != NULL)
    {
        //DBG("sendCtrl Start");
        ret = libusb_control_transfer(dev_handle, LIBUSB_REQUEST_TYPE_VENDOR + type, request, 0, 0, buffer, len, 1);
        //DBG("sendCtrl End");

        if (ret < 0)
        {
            DBG("sendCtrl %02X fail ret:%d", (uint8_t)request , ret);
            return -1;
        }
        else
        {
            DBG("sendCtrl %02X success. ret:%d", (uint8_t)request , ret);
            return 0;
        }
    }

    return -1;
}

int OPENVIO::camRecvStart()
{
    isCamRecv = true;
    camThread->start();
    return true;
}

int OPENVIO::camRecvStop()
{
    isCamRecv = false;
    //camThread->quit();
    camThread->wait();

    // isCamRecv = false;
    // camThread->waitClose();
    return true;
}

int OPENVIO::camStart()
{
    DBG("cam start");
    isCamRecv = true;
    camThread->start();
    ctrlCamStatus(1);
    return true;
}

int OPENVIO::camStop()
{
    int close_try_cnt = 0;

    while (ctrlCamStatus(0) != 0 && close_try_cnt < 4)
    {
        close_try_cnt++;
    }

    isCamRecv = false;
    camThread->waitClose();

    return true;
}

int OPENVIO::ctrlCamStatus(uint8_t state)
{
    uint8_t ret = 0;

    ctrl_buffer[0] = state;
    ret = sendCtrl(REQUEST_SET_CAMERA_STATUS, LIBUSB_ENDPOINT_OUT, ctrl_buffer, 1);
    if (ret < 0)
    {
        return -1;
    }
    ret = getCameraStatus();
    if (ret < 0)
    {
        return -1;
    }
    else if (camStatus != state)
    {
        return -1;
    }

    //DBG("cam ctrl start success");

    return 0;
}

int OPENVIO::ctrlReboot(uint8_t boot)
{
    uint8_t ret = 0;

    ctrl_buffer[0] = boot;
    ret = sendCtrl(REQUEST_SET_REBOOT, LIBUSB_ENDPOINT_OUT, ctrl_buffer, 1);
    if (ret < 0)
    {
        return -1;
    }
    return 0;
}

int OPENVIO::getVersion()
{
    uint8_t ret = 0;

    ret = sendCtrl(REQUEST_GET_VERSION, LIBUSB_ENDPOINT_IN, ctrl_buffer, 128);
    if (ret < 0)
    {
        return -1;
    }
    else
    {
        version[0] = ctrl_buffer[0];
        version[1] = ctrl_buffer[1];
        version[2] = ctrl_buffer[2];

        //mlog->show("get version v" +
        //           QString::number(version[0]) + "." +
        //           QString::number(version[1]) + "." +
        //           QString::number(version[2]));
    }

    return 0;
}

int OPENVIO::IMUStart()
{
    //DBG("imu start");
    isIMURecv = true;
    imuThread->start();
    ctrlIMUStart();
    return true;
}

int OPENVIO::IMUStop()
{
    int close_try_cnt = 0;

    while (ctrlIMUStop() != 0 && close_try_cnt < 4)
    {
        close_try_cnt++;
    }

    isIMURecv = false;
    imuThread->waitClose();

    DBG("imu stop");

    return true;
}

int OPENVIO::ctrlIMUStart()
{
    // uint8_t ret = 0;

    // ret = sendCtrl(REQUEST_IMU_START, 0, 0, ctrl_buffer);
    // if ((ret > 0) && (ctrl_buffer[0] == 'S'))
    // {
    //     imuStatus = SENSOR_STATUS_RUNNING;
    //     return 0;
    // }
    return 0;
}

int OPENVIO::ctrlIMUStop()
{

    // uint8_t ret = 0;

    // ret = sendCtrl(REQUEST_IMU_STOP, 0, 0, ctrl_buffer);
    // if ((ret > 0) && (ctrl_buffer[0] == 'S'))
    // {
    //     imuStatus = SENSOR_STATUS_STOP;
    //     return 0;
    // }
    return 0;
}

int OPENVIO::ctrlCamSyncStatus(uint8_t state)
{
    uint8_t ret = 0;

    ctrl_buffer[0] = state;
    ret = sendCtrl(REQUEST_CAMERA_SET_SYNC_STATUS, LIBUSB_ENDPOINT_OUT, ctrl_buffer, 1);
    if (ret < 0)
    {
        return -1;
    }
    ret = getCameraStatus();
    if (ret < 0)
    {
        return -1;
    }
    else if (state != is_sync_start)
    {
        return -1;
    }
    return 0;
}

int OPENVIO::ctrlCamSyncMode(uint8_t mode)
{
    uint8_t ret = 0;

    ctrl_buffer[0] = mode;

    ret = sendCtrl(REQUEST_CAMERA_SET_SYNC_MODE, LIBUSB_ENDPOINT_OUT, ctrl_buffer, 1);
    if (ret < 0)
    {
        return -1;
    }

    ret = getCameraStatus();
    if (ret < 0)
    {
        return -1;
    }
    else if (mode != is_sync_mode)
    {
        return -1;
    }
    return 0;
}

int OPENVIO::ctrlCamFps(uint8_t fps)
{
    uint8_t ret = 0;

    ctrl_buffer[0] = fps;

    ret = sendCtrl(REQUEST_CAMERA_SET_FPS, LIBUSB_ENDPOINT_OUT, ctrl_buffer, 1);
    if (ret < 0)
    {
        return -1;
    }

    ret = getCameraStatus();
    if (ret < 0)
    {
        return -1;
    }
    else if (fps != camera_fps)
    {
        return -1;
    }

    return 0;
}

int OPENVIO::ctrlInfraredPwm(uint8_t pwm)
{
    uint8_t ret = 0;

    ctrl_buffer[0] = pwm;

    ret = sendCtrl(REQUEST_SET_INFRARED_PWM, LIBUSB_ENDPOINT_OUT, ctrl_buffer, 1);
    if (ret < 0)
    {
        return -1;
    }

    ret = getCameraStatus();
    if (ret < 0)
    {
        return -1;
    }
    else if (pwm != infrared_pwm)
    {
        return -1;
    }

    return 0;
}

int OPENVIO::ctrlCamSetFrameSizeNum(uint16_t num)
{

    // uint8_t ret = 0;

    // ret = sendCtrl(REQUEST_CAMERA_SET_FRAME_SIZE_NUM, num, 0, ctrl_buffer);
    // if ((ret >= 0) && (ctrl_buffer[0] == 'S'))
    // {
    //     DBG("ret:%d\t id:%d \t bpp:%d\t size:%d\t pixformat:%d", ret, ctrl_buffer[1], ctrl_buffer[3], ctrl_buffer[2], ctrl_buffer[4]);
    //     cam_id = ctrl_buffer[1];
    //     img.gs_bpp = ctrl_buffer[3];
    //     img.setImgSize(ctrl_buffer[2]);
    //     pixformat = (pixformat_t)ctrl_buffer[4];

    //     return 0;
    // }
    return 0;
}

int OPENVIO::getCameraStatus()
{
    int ret = 0;
    ret = sendCtrl(REQUEST_GET_CAMERA_STATUS, LIBUSB_ENDPOINT_IN, ctrl_buffer, 13);
    if (ret < 0)
    {
        return -1;
    }
    else
    {
        recv_index = 0;
        camStatus = (enum SENSOR_STATUS)ctrl_buffer[0];
        cam_id = ctrl_buffer[1];
        img.gs_bpp = ctrl_buffer[3];
        img.setImgSize(ctrl_buffer[2]);
        pixformat = (pixformat_t)ctrl_buffer[4];
        exposure = (int)((ctrl_buffer[5] << 24) | (ctrl_buffer[6] << 16) | (ctrl_buffer[7] << 8) | (ctrl_buffer[8] << 0));
        is_sync_mode = ctrl_buffer[9];
        is_sync_start = ctrl_buffer[10];
        camera_fps = ctrl_buffer[11];
        infrared_pwm = ctrl_buffer[12];

        DBG("run:%d  id:%d  bpp:%d  size:%d  pixformat:%d  exposure:%d", ctrl_buffer[0],
            ctrl_buffer[1], ctrl_buffer[3], ctrl_buffer[2], ctrl_buffer[4], exposure);
        DBG("is_sync_mode:%d  is_sync_start:%d  camera_fps:%d infrared_pwm:%d", is_sync_mode,
            is_sync_start, camera_fps, infrared_pwm);

        emit getCameraStatusSignal();
    }
    return 0;
}

int OPENVIO::ctrlCamSetExposure(int value)
{
    uint8_t ret = 0;

    ctrl_buffer[0] = (uint8_t)(value >> 24);
    ctrl_buffer[1] = (uint8_t)(value >> 16);
    ctrl_buffer[2] = (uint8_t)(value >> 8);
    ctrl_buffer[3] = (uint8_t)(value >> 0);

    ret = sendCtrl(REQUEST_CAMERA_SET_EXPOSURE, LIBUSB_ENDPOINT_OUT, ctrl_buffer, 4);
    if (ret < 0)
    {
        return -1;
    }

    ret = getCameraStatus();
    if (ret < 0)
    {
        return -1;
    }
    else if (exposure != value)
    {
        return -1;
    }

    return 0;
}