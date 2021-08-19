#ifndef OPENVIO_H
#define OPENVIO_H

#include "workspace.h"
#include <QObject>
#include <QApplication>

#include "libusb.h"
#include <QStandardItemModel>
#include "MuItemCam.h"
#include "image.h"
#include "findstr.h"

class FormCamConfig;
#include "FormCamConfig.h"

class FormVioWindow;
#include "formviowindow.h"

class FirmwareUpgrade;
#include "firmwareUpgrade.h"

class USBThread;
#include "usbthread.h"

class VisionProcess;
#include "VisionProcess.h"

class CamProcess;
#include "CamProcess.h"

//OUT
#define REQUEST_SET_CAMERA_STATUS           0xA0
#define REQUEST_SET_REBOOT 					0x01

//IN
#define REQUEST_GET_CAMERA_STATUS 	        0x10
#define REQUEST_GET_VERSION 		        0x00

#define REQUEST_CAMERA_SET_FRAME_SIZE_NUM   0xA2
#define REQUEST_CAMERA_SET_EXPOSURE         0xA3
#define REQUEST_CAMERA_SET_SYNC_STATUS 	    0xA4
#define REQUEST_CAMERA_SET_SYNC_MODE 	    0xA5
#define REQUEST_CAMERA_SET_FPS              0xA6
#define REQUEST_SET_INFRARED_PWM            0xA7

#define REQUEST_IMU_START                   0xB0
#define REQUEST_IMU_STOP                    0xB1

class OPENVIO : public QObject
{
    Q_OBJECT
private:
    QThread camProcessThread;
    
public:
    CamProcess *camProcess;

    USBThread *camThread,*imuThread;
    enum SENSOR_STATUS camStatus,imuStatus;
    int recv_len_count = 0;

    int isCamRecv=false,isIMURecv=false;
    unsigned char ctrl_buffer[128];
    int camRecvLen,imuRecvLen,recv_index;
    
    unsigned int recv_count_1s = 0,frame_fps = 0,imu_hz = 0;
    Image img;
    unsigned char cam_id;

    unsigned char version[3];
    
    int exposure;
    uint8_t is_sync_mode;
    uint8_t is_sync_start;
    uint8_t camera_fps;
    uint8_t infrared_pwm;

    pixformat_t pixformat;
    QString saveImagePath;
    FormVioWindow *formVioWindow = nullptr;
    
    int saveCount = 0;
    
    MuItemCamData itemCamData;
    QStandardItem *pItem;
    char idStr[128];
    char *idShort = &idStr[16];
    char productStr[128];
    enum OPENVIO_TYPE type = TYPE_NULL;

    QString name = "";
    int number = -1;

    libusb_device_handle *dev_handle = NULL;
    libusb_device *dev;
    uint8_t devAddr;
    QStandardItemModel *pModelOpenvio;
    
    //int row;

    OPENVIO(libusb_device *dev);
    ~OPENVIO();

    int open(void);
    int close(void);
    
    void removeReady(void);
    int getRow(void);
    void setItem(QStandardItemModel *pModelOpenvio);
    void setStatus(QString status);
    void setSpeed(QString speed);
    void setNumber(int number);
    void CamRecv(void);
    void IMURecv(void);
    int sendCtrl(char request, uint8_t type, unsigned char *buffer,uint16_t len);
    bool sendBulk(unsigned char * buffer,int len);
    int ctrlCamStatus(uint8_t state);

    int ctrlIMUStart();
    int ctrlIMUStop();
    int ctrlCamSetFrameSizeNum(uint16_t num);
    int ctrlCamSetExposure(int value);
    
    int camStart();
    int IMUStart();
    int camStop();
    int IMUStop();

    int camRecvStart();
    int camRecvStop();
    
    int recvBulk(unsigned char *buffer, int len);
    void removeItem(void);
    int getVersion();
    int ctrlReboot(uint8_t boot);
    int getCameraStatus();
    int ctrlCamSyncStatus(uint8_t state);
    int ctrlCamSyncMode(uint8_t mode);
    int ctrlCamFps(uint8_t fps);
    int ctrlInfraredPwm(uint8_t pwm);
signals:
    void camSignals(int index);
    void imuSignals(int index);   
    void closeSignals(void);   
    void setStatusSignal(QString str); 
    void setSpeedSignal(QString speed); 
    void getCameraStatusSignal(void); 
private slots:
    void closeSlot(void);    
    void setStatusSlot(QString str); 
    void setSpeedSlot(QString speed);
};

#endif // SETTING_H
