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
#include <QImage>
#include <QHash>

#include "GLPoint.h"
#include "MultipleViewTriangulation.h"


typedef struct CAMERA_RESULT{
    int camIndex;
    int pointNum;
    qint64 time;
    QString path;
    QImage image;
    double x[PT_NUM_MAX];
    double y[PT_NUM_MAX];
    QHash<POINT_STATE, GLPoint *> *hPoint; 
}CAMERA_RESULT;

class VisionProcess:public QObject        
{
    Q_OBJECT
private:
    int camNum = 0;
    
public:
    int count = 0;
    bool isCapImage = false;
    
    CAMERA_RESULT camResult[CAM_NUM_MAX];

    qint64 lastTime[CAM_NUM_MAX],lastAllTime;
    qint64 saveLastTime = 0;

    MultipleViewTriangulation multipleViewTriangulation;
    VisionProcess(QObject* parent = nullptr);
    void init(int camNum);
public slots:
    void positionSlot(CAMERA_RESULT result);  
signals:

};
 
#endif // VISIONPROCESS_H