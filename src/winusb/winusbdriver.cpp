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

                        if(vio->type == TYPE_OPENVIO)
                        {
                            if(vio->getCameraStatus() == 0)
                            {
                                connect(vio->camProcess, SIGNAL(positionSignals(CAMERA_RESULT)), visionProcess, SLOT(positionSlot(CAMERA_RESULT)));
                                openvioList->append(vio);
                                vio->setItem(pModelOpenvio);
                                vio->ctrlCamStatus(0);
                                vio->camRecvStart();
                                isNew = true;
                            }
                        }else if(vio->type == TYPE_BOOTLOADER)
                        {
                            openvioList->append(vio);
                            vio->setItem(pModelOpenvio);
                            
                            isNew = true;
                        }
                    }else{
                        vio->close();
                    }
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