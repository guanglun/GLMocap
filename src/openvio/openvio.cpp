#include "openvio.h"

OPENVIO::OPENVIO(libusb_device *dev)
{
    this->dev = dev;
    pItem = new QStandardItem();

    camThread = new USBThread();
    imuThread = new USBThread();

    upgrade = new FirmwareUpgrade(this);

    camThread->init(this, "cam");
    imuThread->init(this, "imu");

    ctrl_buffer = (unsigned char *)malloc(1024);

    camStatus = SENSOR_STATUS_STOP;
    imuStatus = SENSOR_STATUS_STOP;

    connect(this, SIGNAL(closeSignals()), this, SLOT(closeSlot()));
}

bool OPENVIO::open(void)
{
    int ret = 0;

    if (is_open)
    {
        DBG("already open");
        return true;
    }

    dev_handle = NULL;

    // ret = libusb_init(&m_libusb_context);
    // if (ret < 0)
    // {
    //     mlog->show("libusb init fail " + ret);
    //     goto init_fail;
    // }
    // else
    // {
    //     mlog->show("libusb init success");
    // }

    libusb_open(dev, &dev_handle);
    if (dev_handle == NULL)
    {
        DBG("device open fail " + ret);
        goto open_fail;
    }
    else
    {
        DBG("device open success");
    }

    ret = libusb_claim_interface(dev_handle, 0);
    if (ret < 0)
    {
        DBG("claim interface fail " + ret);
        goto claim_fail;
    }
    else
    {
        DBG("claim interface success");
    }

    is_open = true;

    DBG("open success");

    //emit sendStatusSignals(USB_MSG_OPEN_SUCCESS);
    //ctrlCamStart();

    return true;

close_fail:
    is_open = false;
claim_fail:
    libusb_close(dev_handle);
open_fail:
init_fail:
    DBG("open fail");
    //emit sendStatusSignals(USB_MSG_OPEN_FAIL);
    return false;
}

void OPENVIO::CamRecv(void)
{
    DBG("cam recv start");
    int ret = 0;
    int img_index = 0;
    int recv_head_status = 0;
    int findRet = 0;
    FindStr findStr;
    uint8_t head_tmp[1024];
    findStr.config((unsigned char *)"CAMERA", 6);
    while (isCamRecv)
    {
        //        if (recv_head_status == 0)
        //            ret = libusb_bulk_transfer(dev_handle, CAM_EPADDR, (unsigned char *)(head_tmp), 1024, &camRecvLen, 1000);
        //        else
        ret = libusb_bulk_transfer(dev_handle, CAM_EPADDR, (unsigned char *)(img.img[img_index] + recv_index), 512 * 1024, &camRecvLen, 1000);
        if (ret < 0)
        {
            if (ret != -7 && ret != -9)
            {
                DBG("%s cam recv error %d", name.toStdString().c_str(), ret);
                //emit disconnectSignals();
                break;
            }
            else
            {
                //DBG("cam recv time out");
            }
        }
        else if (camStatus == SENSOR_STATUS_RUNNING)
        {
            //DBG("cam recv %d %d %d",recv_head_status,camRecvLen,recv_index);
            recv_count_1s += camRecvLen;

            if ((recv_head_status == 0) && (camRecvLen == 12))
            {
                findRet = findStr.input(img.img[img_index], camRecvLen);
                if (findRet > 0)
                {
                    recv_head_status = 1;
                    memcpy(img.time[img_index], img.img[img_index] + 6, 6);
                }
            }
            else if (camRecvLen == 12)
            {
                findRet = findStr.input(img.img[img_index] + recv_index, camRecvLen);
                if (findRet > 0)
                {
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
                DBG("%s cam recv error len %d", name.toStdString().c_str(), camRecvLen);
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

    ret = libusb_bulk_transfer(dev_handle, 0x01, buffer, len, &recvLen, 10);

    if (ret < 0)
    {
        DBG("libusb_control_transfer fail");
        return false;
    }
    else
    {
        DBG("libusb_control_transfer success. ret:%d buffer:%c", ret, buffer[0]);

        return true;
    }
}

void OPENVIO::setItem(QStandardItemModel *pModelOpenvio)
{
    this->pModelOpenvio = pModelOpenvio;
    itemCamData.id = idStr;
    itemCamData.status = "wait";
    itemCamData.name = setting->getNameById(idStr);
    itemCamData.type = type;
    this->name = itemCamData.name;

    pItem->setData(QVariant::fromValue(itemCamData), Qt::UserRole + 1);
    pModelOpenvio->appendRow(pItem);
    row = pModelOpenvio->rowCount() - 1;
}

void OPENVIO::setStatus(QString status)
{
    itemCamData.status = status;

    pModelOpenvio->item(row, 0)->setData(QVariant::fromValue(itemCamData));
}

void OPENVIO::setName(QString name)
{
    itemCamData.name = name;
    this->name = itemCamData.name;
    pModelOpenvio->item(row, 0)->setData(QVariant::fromValue(itemCamData));
    setting->setNameById(idStr, name);
}

int OPENVIO::close(void)
{
    emit closeSignals();
}

void OPENVIO::closeSlot(void)
{
    int close_try_cnt = 0;

    if (is_open)
    {
        close_try_cnt = 0;
        while (ctrlCamStop() != 0 && close_try_cnt > 2)
        {
            close_try_cnt++;
            mlog->show("close cam fail");
        }

        close_try_cnt = 0;
        while (ctrlIMUStop() != 0 && close_try_cnt > 2)
        {
            close_try_cnt++;
            mlog->show("close imu fail");
        }

        is_open = false;

        //DBG("closeSlot");

        //libusb_release_interface(dev_handle, 0);

        //        libusb_close(dev_handle);

        //        libusb_release_interface(dev_handle, 0);

        libusb_close(dev_handle);
        //libusb_exit(m_libusb_context);

        mlog->show("close success");
        //emit sendStatusSignals(USB_MSG_CLOSE_SUCCESS);
    }
}

#define REQUEST_CAMERA_START 0xA0
#define REQUEST_CAMERA_STOP 0xA1
#define REQUEST_CAMERA_SET_FRAME_SIZE_NUM 0xA2
#define REQUEST_CAMERA_SET_EXPOSURE 0xA3

#define REQUEST_IMU_START 0xB0
#define REQUEST_IMU_STOP 0xB1

int OPENVIO::sendCtrl(char request, uint16_t wValue, uint16_t wIndex, unsigned char *buffer)
{
    int ret = 0;
    if (is_open)
    {

        ret = libusb_control_transfer(dev_handle, LIBUSB_REQUEST_TYPE_VENDOR + LIBUSB_ENDPOINT_IN, request, wValue, wIndex, buffer, 128, 1000);

        if (ret < 0)
        {
            DBG("libusb_control_transfer fail");
            return -1;
        }
        else
        {
            DBG("libusb_control_transfer success. ret:%d buffer:%c", ret, buffer[0]);

            return ret;
        }
    }

    return -1;
}

int OPENVIO::camStart()
{
    DBG("cam start");
    isCamRecv = true;
    camThread->start();
    ctrlCamStart();
    return true;
}

int OPENVIO::camStop()
{
    int close_try_cnt = 0;

    while (ctrlCamStop() != 0 && close_try_cnt < 4)
    {
        close_try_cnt++;
    }

    isCamRecv = false;
    camThread->waitClose();

    DBG("cam stop");

    return true;
}

int OPENVIO::ctrlCamStart()
{
    uint8_t ret = 0;
    recv_index = 0;

    ret = sendCtrl(REQUEST_CAMERA_START, 0, 0, ctrl_buffer);
    if ((ret >= 0) && (ctrl_buffer[0] == 'S'))
    {
        camStatus = SENSOR_STATUS_RUNNING;
        recv_index = 0;
        DBG("ret:%d\t id:%d \t bpp:%d\t size:%d\t pixformat:%d", ret, ctrl_buffer[1], ctrl_buffer[3], ctrl_buffer[2], ctrl_buffer[4]);
        cam_id = ctrl_buffer[1];
        img.gs_bpp = ctrl_buffer[3];
        img.setImgSize(ctrl_buffer[2]);
        pixformat = (pixformat_t)ctrl_buffer[4];

        return 0;
    }

    return -1;
}

int OPENVIO::ctrlCamStop()
{
    uint8_t ret = 0;

    ret = sendCtrl(REQUEST_CAMERA_STOP, 0, 0, ctrl_buffer);

    if ((ret > 0) && (ctrl_buffer[0] == 'S'))
    {
        camStatus = SENSOR_STATUS_STOP;
        return 0;
    }

    return -1;
}

int OPENVIO::IMUStart()
{
    DBG("imu start");
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
    uint8_t ret = 0;

    ret = sendCtrl(REQUEST_IMU_START, 0, 0, ctrl_buffer);
    if ((ret > 0) && (ctrl_buffer[0] == 'S'))
    {
        imuStatus = SENSOR_STATUS_RUNNING;
        return 0;
    }
    return -1;
}

int OPENVIO::ctrlIMUStop()
{

    uint8_t ret = 0;

    ret = sendCtrl(REQUEST_IMU_STOP, 0, 0, ctrl_buffer);
    if ((ret > 0) && (ctrl_buffer[0] == 'S'))
    {
        imuStatus = SENSOR_STATUS_STOP;
        return 0;
    }
    return -1;
}

int OPENVIO::ctrlCamSetFrameSizeNum(uint16_t num)
{

    uint8_t ret = 0;

    ret = sendCtrl(REQUEST_CAMERA_SET_FRAME_SIZE_NUM, num, 0, ctrl_buffer);
    if ((ret >= 0) && (ctrl_buffer[0] == 'S'))
    {
        DBG("ret:%d\t id:%d \t bpp:%d\t size:%d\t pixformat:%d", ret, ctrl_buffer[1], ctrl_buffer[3], ctrl_buffer[2], ctrl_buffer[4]);
        cam_id = ctrl_buffer[1];
        img.gs_bpp = ctrl_buffer[3];
        img.setImgSize(ctrl_buffer[2]);
        pixformat = (pixformat_t)ctrl_buffer[4];

        return 0;
    }
    return -1;
}

int OPENVIO::ctrlCamSetExposure(int value)
{

    uint8_t ret = 0;
    uint16_t wValue, wIndex;
    wValue = (uint16_t)(value >> 16);
    wIndex = (uint16_t)(value >> 0);

    ret = sendCtrl(REQUEST_CAMERA_SET_EXPOSURE, wValue, wIndex, ctrl_buffer);
    if ((ret >= 0) && (ctrl_buffer[0] == 'S'))
    {
        DBG("ret:%d\t", ret);

        return 0;
    }
    return -1;
}