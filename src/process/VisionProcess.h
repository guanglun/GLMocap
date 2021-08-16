#ifndef VISIONPROCESS_H
#define VISIONPROCESS_H

#include "workspace.h"
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include <QMetaType>

typedef struct CAMERA_RESULT{
    int camIndex;
    QDateTime time;
    double x;
    double y;
}CAMERA_RESULT;

class VisionProcess:public QObject        
{
    Q_OBJECT
private:
public:
    VisionProcess(QObject* parent = nullptr);
public slots:
    void positionSlot(CAMERA_RESULT result);  
signals:

};
 
#endif // CAMPROCESS_H