#include "winusbdriver.h"
#include <QThread>

WinUSBDriver::WinUSBDriver()
{
    // camThread = new USBThread();
    // imuThread = new USBThread();

    // camThread->init(this, "cam");
    // imuThread->init(this, "imu");

    // ctrl_buffer = (unsigned char *)malloc(1024);

    // camStatus = SENSOR_STATUS_STOP;
    // imuStatus = SENSOR_STATUS_STOP;

    libusb_init(&m_libusb_context);

    // connect(this, SIGNAL(closeSignals()), this, SLOT(closeSlot()));
    // connect(this, SIGNAL(openSignals(int, int)), this, SLOT(openSlot(int, int)));
    connect(this, SIGNAL(scanSignals()), this, SLOT(scanSlot()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer->start(1000);
}

WinUSBDriver::~WinUSBDriver()
{
    free(ctrl_buffer);
}

void WinUSBDriver::setModule(QStandardItemModel *pModelOpenvio)
{
    this->pModelOpenvio = pModelOpenvio;
}

void WinUSBDriver::setOpenvioList(QList<OPENVIO *> *openvioList)
{
    this->openvioList = openvioList;
}

void WinUSBDriver::open(int vid, int pid)
{
    emit openSignals(vid, pid);
}

void WinUSBDriver::scan(void)
{
    emit scanSignals();
}

void WinUSBDriver::scanSlot(void)
{
    //autoScan();
}

void WinUSBDriver::onTimeOut()
{
    autoScan();
}

void WinUSBDriver::autoScan(void)
{
    //DBG("start scan");

    struct libusb_device_descriptor desc;
    struct libusb_config_descriptor *conf;
    struct libusb_device *device;
    libusb_device_handle *handle = NULL;
    int config = 0;
    int ret;
    int ii, i;

    int status;

    ssize_t num_devs, j, k;
    int size = openvioList->size();

    num_devs = libusb_get_device_list(m_libusb_context, &list);

    for (i = 0; i < size; i++)
    {

        for (ii = 0; ii < num_devs; ii++)
        {
            device = list[ii];

            if (openvioList->at(i)->devAddr == libusb_get_device_address(device))
            {
                //openvioList->at(i)->dev = device;
                break;
            }
        }

        if (ii >= num_devs)
        {
            DBG("remove %s %s %d", openvioList->at(i)->productStr, openvioList->at(i)->idShort, openvioList->at(i)->devAddr);
            openvioList->at(i)->removeItem();
            openvioList->removeAt(i);

            i--;
            size--;
            DBG("remove success");
        }
    }

    //openvioList->clear();
    //pModelOpenvio->clear();

    for (i = 0; i < num_devs; ++i)
    {

        device = list[i];
        struct libusb_device_descriptor desc;
        libusb_get_device_descriptor(device, &desc);
        enum OPENVIO_TYPE type = TYPE_NULL;

        if (desc.idVendor == OPENVIO_IDVENDOR && desc.idProduct == OPENVIO_IDPRODUCT)
        {
            type = TYPE_OPENVIO;
        }
        else if (desc.idVendor == OPENVIO_BOOTLOADER_IDVENDOR && desc.idProduct == OPENVIO_BOOTLOADER_IDPRODUCT)
        {
            type = TYPE_BOOTLOADER;
        }

        if (type != TYPE_NULL)
        {
            size = openvioList->size();
            for (ii = 0; ii < size; ii++)
            {
                if (openvioList->at(ii)->devAddr == libusb_get_device_address(device))
                {
                    break;
                }
            }

            if (ii >= size)
            {
                OPENVIO *vio = new OPENVIO(device);
                bool isNew = false;

                vio->dev = device;
                vio->type = type;
                vio->devAddr = libusb_get_device_address(device);

                //DBG("new addr : %d ",vio->devAddr);
                ret = vio->open();
                if(ret < 0)
                {
                    delete vio;
                    break;
                }

                ret = libusb_get_string_descriptor_ascii(vio->dev_handle,
                                                         desc.iSerialNumber,
                                                         (unsigned char *)vio->idStr,
                                                         sizeof(vio->idStr));
                if (ret < 0)
                {
                    DBG("get descriptor fail %d", ret);
                }

                ret = libusb_get_string_descriptor_ascii(vio->dev_handle,
                                                         desc.iProduct,
                                                         (unsigned char *)vio->productStr,
                                                         sizeof(vio->productStr));
                if (ret < 0)
                {
                    DBG("get descriptor fail %d", ret);
                }

                if (QString(vio->productStr).length() >= 7 &&
                    QString(vio->idStr).length() == 24)
                {
                    if (vio->getVersion() != -1)
                    {
                        DBG("found %s %s %d v%d.%d.%d", vio->productStr, vio->idShort, vio->devAddr,vio->version[0],vio->version[1],vio->version[2]);
                        openvioList->append(vio);
                        vio->setItem(pModelOpenvio);
                        isNew = true;
                    }
                    vio->close();
                    
                }
                else
                {
                    vio->close();
                    delete vio;
                }

                if(isNew)
                {
                    emit newSignal(vio);
                }
            }
        }
    }

    //DBG("all found openvio : %d", (int)openvioList->length());

    //libusb_free_device_list(list, 1);
    //libusb_exit(m_libusb_context);

    //return;
}

void WinUSBDriver::CamRecv(void)
{
    DBG("cam recv start");

    int img_index = 0;
    int recv_head_status = 0;
    int findRet = 0;
    FindStr findStr;
    uint8_t head_tmp[1024];
    findStr.config((unsigned char *)"CAMERA", 6);
    while (is_open)
    {
        //        if (recv_head_status == 0)
        //            ret = libusb_bulk_transfer(dev_handle, CAM_EPADDR, (unsigned char *)(head_tmp), 1024, &camRecvLen, 1000);
        //        else
        ret = libusb_bulk_transfer(dev_handle, CAM_EPADDR, (unsigned char *)(img.img[img_index] + recv_index), 512 * 1024, &camRecvLen, 1000);
        if (ret < 0)
        {
            if (ret != -7 && ret != -9)
            {
                DBG("cam recv error %d", ret);
                emit disconnectSignals();
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
                DBG("cam recv error len %d", camRecvLen);
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

void WinUSBDriver::IMURecv(void)
{
    DBG("imu recv start");

    FindStr findStr;
    findStr.config((unsigned char *)"ICMIMU", 6);

    int imu_index = 0;

    while (is_open)
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
                emit imuSignals(imu_index);
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

void WinUSBDriver::send(QByteArray byte)
{
    if (is_open)
    {
        libusb_bulk_transfer(dev_handle, CTRL_EPADDR, (unsigned char *)byte.data(), byte.length(), NULL, 0xFFFF);
    }
}

int WinUSBDriver::close(void)
{
    //emit closeSignals();
}