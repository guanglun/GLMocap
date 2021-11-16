#ifndef MARKSMANAGEMENT_H
#define MARKSMANAGEMENT_H

#include "workspace.h"

#include <QObject>

#include <opencv2/opencv.hpp>
#include "MultipleViewTriangulation.h"

class MarkPoint;
#include "MarkPoint.h"

using namespace std;

class MarksManagement:public QObject        
{
    Q_OBJECT
private:
    int camNum;
    vector<MarkPoint *> freePoint[CAM_NUM_MAX];
public:
    MarksManagement();
    void catchDone(int camNum);
    void inputPoint(int index,vector<cv::Point2f> pt);
public slots:

signals:

};
 
#endif // MARKSMANAGEMENT_H