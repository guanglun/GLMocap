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

class FormVioWindow;
#include "FormVioWindow.h"

class FirmwareUpgrade;
#include "firmwareUpgrade.h"

class USBThread;
#include "usbthread.h"



class OPENVIO : public QObject
{
    Q_OBJECT
private:

    
public:
    boolean isCapImage;
    USBThread *camThread,*imuThread;
    enum SENSOR_STATUS camStatus,imuStatus;
    int recv_len_count = 0;
    bool isShowSpeed = false;
    int isCamRecv=false,isIMURecv=false;
    unsigned char *ctrl_buffer;
    int camRecvLen,imuRecvLen,recv_index;
    bool is_open;
    unsigned int recv_count_1s = 0,frame_fps = 0,imu_hz = 0;
    Image img;
    unsigned char cam_id;

    pixformat_t pixformat;
    QString saveImagePath;
    FormVioWindow *formVioWindow = NULL;

    int saveCount = 0;
    
    MuItemCamData itemCamData;
    QStandardItem *pItem;
    char idStr[128];
    char *idShort = &idStr[16];
    char productStr[128];
    enum OPENVIO_TYPE type = TYPE_NULL;
    QString name;
    libusb_device_handle *dev_handle = NULL;
    libusb_device *dev;
    uint8_t devAddr;
    QStandardItemModel *pModelOpenvio;
    
    //int row;

    OPENVIO(libusb_device *dev);
    bool open(void);
    void setItem(QStandardItemModel *pModelOpenvio);
    void setStatus(QString status);
    void setName(QString name);
    void CamRecv(void);
    void IMURecv(void);
    int sendCtrl(char request, uint16_t wValue,uint16_t wIndex,unsigned char *buffer);
    bool sendBulk(unsigned char * buffer,int len);
    int ctrlCamStart();
    int ctrlCamStop();
    int ctrlIMUStart();
    int ctrlIMUStop();
    int ctrlCamSetFrameSizeNum(uint16_t num);
    int ctrlCamSetExposure(int value);
    int close(void);
    int camStart();
    int IMUStart();
    int camStop();
    int IMUStop();
    int recvBulk(unsigned char *buffer, int len);
    void removeItem(void);
signals:
    void camSignals(int index);
    void imuSignals(int index);   
    void closeSignals(void);   
    void setStatusSignal(QString str); 
private slots:
    void closeSlot(void);    
    void setStatusSlot(QString str); 
};

#endif // SETTING_H
