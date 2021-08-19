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

    qRegisterMetaType<CAMERA_RESULT>("CAMERA_RESULT");
    visionProcess = new VisionProcess(this);
    visionProcess->moveToThread(&visionProcessThread);
    visionProcessThread.start();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer->start(1000);
}

WinUSBDriver::~WinUSBDriver()
{
}

void WinUSBDriver::setModule(QStandardItemModel *pModelOpenvio)
{
    this->pModelOpenvio = pModelOpenvio;
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

    struct libusb_device *device;
    libusb_device_handle *handle = NULL;
    int config = 0;
    int ret;
    int i;

    ssize_t num_devs;
    num_devs = libusb_get_device_list(m_libusb_context, &list);

    it = vioMap.begin();
    while (it != vioMap.end())
    {
        for (i = 0; i < num_devs; i++)
        {
            if (it.key() == libusb_get_device_address(list[i]))
            {
                break;
            }
        }
        if (i >= num_devs)
        {
            DBG("remove %s %s %d", it.value()->productStr, it.value()->idShort, it.value()->devAddr);

            it.value()->removeReady();
            it = vioMap.erase(it);
        }
        else
        {
            it++;
        }
    }

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
            if (vioMap[libusb_get_device_address(device)] == nullptr)
            {
                OPENVIO *vio = new OPENVIO(device);
                bool isNew = false;

                vio->dev = device;
                vio->type = type;
                vio->devAddr = libusb_get_device_address(device);

                //DBG("new addr : %d ",vio->devAddr);
                ret = vio->open();
                if (ret < 0)
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
                    DBG("get iSerialNumber fail %d", ret);
                }else{
                    DBG("get id success : %s", vio->idStr);
                }

                ret = libusb_get_string_descriptor_ascii(vio->dev_handle,
                                                         desc.iProduct,
                                                         (unsigned char *)vio->productStr,
                                                         sizeof(vio->productStr));
                if (ret < 0)
                {
                    DBG("get iProduct fail %d", ret);
                }else{
                    DBG("get product success : %s", vio->productStr);
                }

                if (QString(vio->productStr).length() >= 7 &&
                    QString(vio->idStr).length() == 24)
                {
                    if (vio->getVersion() != -1)
                    {
                        DBG("found %s %s %d v%d.%d.%d", vio->productStr, vio->idShort, vio->devAddr, vio->version[0], vio->version[1], vio->version[2]);

                        if (vio->type == TYPE_OPENVIO)
                        {
                            if (vio->getCameraStatus() == 0)
                            {
                                connect(vio->camProcess, SIGNAL(positionSignals(CAMERA_RESULT)), visionProcess, SLOT(positionSlot(CAMERA_RESULT)));
                                vioMap[vio->devAddr] = vio;
                                vio->setItem(pModelOpenvio);
                                vio->ctrlCamStatus(0);
                                vio->camRecvStart();
                                isNew = true;
                            }
                        }
                        else if (vio->type == TYPE_BOOTLOADER)
                        {
                            //openvioList->append(vio);
                            vioMap[vio->devAddr] = vio;
                            vio->setItem(pModelOpenvio);

                            isNew = true;
                        }
                    }
                    else
                    {
                        vio->close();
                    }
                }
                else
                {
                    vio->close();
                    delete vio;
                }

                if (isNew)
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
    return 0;
}