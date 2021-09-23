#ifndef CALIBRPROCESS_H
#define CALIBRPROCESS_H

#include "workspace.h"
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QWidget>
#include <QDateTime>
#include <QList>

#include "Calibration.h"

class CalibrProcess:public QObject        
{
    Q_OBJECT
private:

    bool isRuning = false;
    bool isStop = false;
    Calibration calibr;
public:
    
    CalibrProcess(QObject* parent = nullptr);

    void stop(void);
public slots:
    void startSlot(void);
    
};
 
#endif // CALIBRPROCESS_H