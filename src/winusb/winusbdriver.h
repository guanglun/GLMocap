#ifndef WINUSBDRIVER_H
#define WINUSBDRIVER_H

#include "workspace.h"
#include <QObject>

#include "libusb.h"

#include "image.h"
#include "findstr.h"
class USBThread;
#include "usbthread.h"

#define CANDLE_MAX_DEVICES 32

#define OV2640_ID           (0x26)
#define OV5640_ID           (0x56)
#define OV7690_ID           (0x76)
#define OV7725_ID           (0x77)
#define OV9650_ID           (0x96)
#define MT9V034_ID          (0x13)
#define LEPTON_ID           (0x54)
#define HM01B0_ID           (0xB0)

typedef enum {
    PIXFORMAT_INVALID = 0,
    PIXFORMAT_BINARY,    // 1BPP/BINARY
    PIXFORMAT_GRAYSCALE, // 1BPP/GRAYSCALE
    PIXFORMAT_RGB565,    // 2BPP/RGB565
    PIXFORMAT_YUV422,    // 2BPP/YUV422
    PIXFORMAT_BAYER,     // 1BPP/RAW
    PIXFORMAT_JPEG,      // JPEG/COMPRESSED
} pixformat_t;

enum{
USB_MSG_OPEN_SUCCESS,
USB_MSG_CLOSE_SUCCESS,
USB_MSG_OPEN_FAIL
};

enum SENSOR_STATUS{
    SENSOR_STATUS_STOP,
    SENSOR_STATUS_WAIT_HEAD,
    SENSOR_STATUS_RUNNING
};

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
    libusb_device_handle *dev_handle = NULL;
    struct libusb_device_descriptor desc;
    libusb_config_descriptor *cfg = NULL;
    libusb_device **usb_list;
    
	struct libusb_config_descriptor* conf;
    struct libusb_transfer* m_xfer;
    
    USBThread *camThread,*imuThread;
    libusb_context* m_libusb_context;

    enum SENSOR_STATUS camStatus,imuStatus;
    
public:
    Image img;
    unsigned char cam_id;
    pixformat_t pixformat;
    unsigned int recv_count_1s = 0,frame_fps = 0,imu_hz = 0;
    
    WinUSBDriver();
    ~WinUSBDriver();
    
    void open(int vid,int pid);
    int close(void);
    static void LIBUSB_CALL completeCallback(libusb_transfer *xfer);
    void send(QByteArray byte);
    void CamRecv(void);
    void IMURecv(void);
    int sendCtrl(char request, uint16_t wValue,uint16_t wIndex,unsigned char *buffer);
    int ctrlCamStart();
    int ctrlCamStop();
    int ctrlIMUStart();
    int ctrlIMUStop();
    int ctrlCamSetFrameSizeNum(uint16_t num);
    int ctrlCamSetExposure(int value);

signals:
    void camSignals(int index);
    void imuSignals(int index);

    void disconnectSignals(void);
    void closeSignals(void);
    void openSignals(int vid,int pid);
    void sendStatusSignals(int msg);
private slots:
    void closeSlot(void);
    void openSlot(int vid,int pid);
};

#endif // WINUSBDRIVER_H
