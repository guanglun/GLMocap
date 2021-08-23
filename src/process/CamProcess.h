#ifndef CAMPROCESS_H
#define CAMPROCESS_H

#include "workspace.h"

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include <QHash>

#include "GLPoint.h"

//class VisionProcess;
#include "VisionProcess.h"

class OPENVIO;
#include "openvio.h"



class CamProcess:public QObject        
{
    Q_OBJECT
private:
    OPENVIO *vio = NULL;    
    Qt::CheckState showFlag = Qt::CheckState::Unchecked;
    QHash<POINT_STATE, GLPoint *> *hPoint;
    CAMERA_RESULT result;
public:
    CamProcess(OPENVIO *vio,QObject* parent = nullptr);
    void cvProcess(QImage qImage,QDateTime time);
    void setShowFlag(Qt::CheckState flag);
public slots:
    void camSlot(int index);

signals:
    void visionImageSignals(QPixmap qImage);  
    void positionSignals(CAMERA_RESULT result);  
};
 
#endif // CAMPROCESS_H