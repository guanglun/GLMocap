#ifndef CTRLPROCESS_H
#define CTRLPROCESS_H

#include <QObject>
#include<QDebug>
#include<QThread>
#include <QWidget>
#include <QDateTime>
#include <QList>

class OPENVIO;
#include "openvio.h"

class CtrlProcess:public QObject        
{
    Q_OBJECT
private:
    OPENVIO *vio = NULL;    
    int count = 0;
public:
    QList<OPENVIO *> *vioList;
    OPENVIO *setvio;

    CtrlProcess(QObject* parent = nullptr);
    void setVio(QList<OPENVIO *> *vioList,OPENVIO *setvio);
    OPENVIO * getEach();
    int getLen(void);
public slots:
    void setExposureSlot(int exposure);
    void syncSlot(void);
    void ctrlCamStatusSlot(unsigned char state);
    void ctrlCamSyncStatusSlot(unsigned char state);
    void ctrlCamSyncModeSlot(unsigned char mode);
    void ctrlCamFpsSlot(unsigned char fps);    
    void ctrlInfraredPwmSlot(unsigned char pwm);    
};
 
#endif // CAMPROCESS_H