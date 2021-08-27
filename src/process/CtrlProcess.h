#ifndef CTRLPROCESS_H
#define CTRLPROCESS_H

#include "workspace.h"
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QWidget>
#include <QDateTime>
#include <QList>

class OPENVIO;
#include "openvio.h"

class CtrlProcess:public QObject        
{
    Q_OBJECT
private:
    int count = 0;
    OPENVIO *vio = NULL;    
public:
    QMap<uint8_t, OPENVIO*> *vioMap;
    QMap<uint8_t, OPENVIO*>::Iterator it;
    OPENVIO *setvio;

    CtrlProcess(QObject* parent = nullptr);
    void setVio(QMap<uint8_t, OPENVIO*> *vioMap,OPENVIO *setvio);
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
    void ctrlMultemCamStartSlot(void);
    void ctrlMultemCamStopSlot(void);
};
 
#endif // CAMPROCESS_H