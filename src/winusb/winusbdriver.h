#ifndef WINUSBDRIVER_H
#define WINUSBDRIVER_H

#include "workspace.h"
#include <QObject>
#include <QThread>
#include <QList>

#include <QStandardItemModel>
#include "MuItemCam.h"

#include "libusb.h"

#include "image.h"
#include "findstr.h"

#include <QMetaType>

class VisionProcess;
#include "VisionProcess.h"

class OPENVIO;
#include "openvio.h"

class USBThread;
#include "usbthread.h"


#define OPENVIO_IDVENDOR                2012
#define OPENVIO_IDPRODUCT               2012

#define OPENVIO_BOOTLOADER_IDVENDOR     2012
#define OPENVIO_BOOTLOADER_IDPRODUCT    2013

#define CANDLE_MAX_DEVICES 32

#define OV2640_ID           (0x26)
#define OV5640_ID           (0x56)
#define OV7690_ID           (0x76)
#define OV7725_ID           (0x77)
#define OV9650_ID           (0x96)
#define MT9V034_ID          (0x13)
#define LEPTON_ID           (0x54)
#define HM01B0_ID           (0xB0)


#define CTRL_EPADDR 0x01
#define CAM_EPADDR  0x81
#define IMU_EPADDR  0x82

#define USB_TIMEOUT 10000 //传输数据的时间延迟

class WinUSBDriver : public QObject
{
    Q_OBJECT
private:
    int recv_len_count = 0;
    
    int ret;
    
    unsigned char *ctrl_buffer;
    int camRecvLen,imuRecvLen,recv_index;
    bool is_open;
    
    libusb_device **devs,*dev;
    libusb_device_handle *dev_handle = nullptr;
    struct libusb_device_descriptor desc;
    libusb_config_descriptor *cfg = nullptr;
    libusb_device **usb_list;
    
	struct libusb_config_descriptor* conf;
    struct libusb_transfer* m_xfer;
    
    USBThread *camThread,*imuThread;
    libusb_context* m_libusb_context;

    enum SENSOR_STATUS camStatus,imuStatus;
    

    QThread visionProcessThread;
public:
    QTimer *timer;
    VisionProcess *visionProcess;
    QStandardItemModel *pModelOpenvio;
    Image img;
    unsigned char cam_id;
    pixformat_t pixformat;
    unsigned int recv_count_1s = 0,frame_fps = 0,imu_hz = 0;
    QMap<uint8_t, OPENVIO*> vioMap; 
    QMap<uint8_t, OPENVIO*>::Iterator it;

    libusb_device **list;

    WinUSBDriver();
    ~WinUSBDriver();
    
    void open(int vid,int pid);
    int close(void);
    static void LIBUSB_CALL completeCallback(libusb_transfer *xfer);
    void send(QByteArray byte);
    void scan(void);
    void autoScan();
    void setModule(QStandardItemModel *pModelOpenvio);
    void closeDevice(void);
signals:
    void camSignals(int index);
    void imuSignals(int index);

    void disconnectSignals(void);
    void closeSignals(void);
    void openSignals(int vid,int pid);
    void sendStatusSignals(int msg);
    void scanSignals(void);
    
    void newSignal(OPENVIO *vio);
    void removeSignal(OPENVIO *vio);
private slots:
    //void closeSlot(void);
    //void openSlot(int vid,int pid);
    void scanSlot(void);
    void onTimeOut(); 
};

#endif // WINUSBDRIVER_H
