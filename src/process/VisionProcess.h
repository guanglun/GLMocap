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

#include "MarksManagement.h"
#include "PX4Thread.h"

using namespace std;

typedef enum MATCH_STATE{
    MATCH_IDLE,
    MATCH_START,
    MATCH_ING,
    MATCH_OK,
    MATCH_TRACK,
}MATCH_STATE;

typedef enum CAL_STATE{
    CAL_IDLE,
    CAL_START,
    CAL_OK,
}CAL_STATE;

typedef enum FIND_MODULE_STATE{
    FIND_MODULE_IDLE,
    FIND_MODULE_START,
    FIND_MODULE_OK,
}FIND_MODULE_STATE;

typedef struct CAMERA_RESULT{
    int camIndex;
    int pointNum;
    qint64 time;
    QString path;
    QImage image;
    double x[PT_NUM_MAX];
    double y[PT_NUM_MAX];
    QHash<POINT_STATE, GLPoint *> *hPoint; 
    vector<GLPoint *> vPoint;
}CAMERA_RESULT;

class VisionProcess:public QObject        
{
    Q_OBJECT
private:
    
    int camNum = 0;
    int pointNum = 0;
    PX4Thread *px4; 
    
    Vector3d eulerAnglesDrone;

    int matchPoint(void);
    int checkVPointSize(void);
    int calibrateGND(vector<GLPoint *> *vPoint);
    int findDroneModule(vector<GLPoint *> *vPoint);
    int onMatching(void);
    void forloop(int pm,int cm);
public:
    MarksManagement *mm;
    MATCH_STATE matchState = MATCH_IDLE;
    CAL_STATE calGNDstate = CAL_IDLE;
    FIND_MODULE_STATE findDroneState = FIND_MODULE_IDLE;


    int count = 0;
    bool isCapImage = false;
    
    
    CAMERA_RESULT camResult[CAM_NUM_MAX];

    qint64 lastTime[CAM_NUM_MAX],lastAllTime;
    qint64 saveLastTime = 0;

    MultipleViewTriangulation multipleViewTriangulation;
    VisionProcess(PX4Thread *px4,QObject* parent = nullptr);
    Vector3d *triangulation(void);
    void init(int camNum);
public slots:
    void positionSlot(CAMERA_RESULT result);  
signals:
    void onXYZSignals(Vector3d *Xr,Vector3d *pos,int size);
};
 
#endif // VISIONPROCESS_H