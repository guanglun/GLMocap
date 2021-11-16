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

class VisionProcess;
#include "VisionProcess.h"

class OPENVIO;
#include "openvio.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class CamProcess:public QObject        
{
    Q_OBJECT
private:
    
    
    OPENVIO *vio = nullptr;    
    Qt::CheckState showFlag = Qt::CheckState::Unchecked;
    QHash<POINT_STATE, GLPoint *> *hPoint;
    vector<GLPoint *> vPoint;
    CAMERA_RESULT result;

    void match(vector<Point2f> centers);

public:
    int index;
    CamProcess(OPENVIO *vio,QObject* parent = nullptr);
    void cvProcess(QImage qImage,QDateTime time);
    void setShowFlag(Qt::CheckState flag);
    void searchMarks(Mat image, vector<Point2f> &points);
    void searchMarks2(Mat image, vector<Point2f> &points);
    void emitImage(QImage image,QDateTime time);
public slots:
    void camSlot(int index);
    void imageSlot(QImage image,QDateTime time);
signals:
    void imageSignal(QImage image,QDateTime time);
    void visionImageSignals(QPixmap qImage);  
    void positionSignals(CAMERA_RESULT result);  
};
 
#endif // CAMPROCESS_H