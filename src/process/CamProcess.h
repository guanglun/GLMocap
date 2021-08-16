#ifndef CAMPROCESS_H
#define CAMPROCESS_H

#include <QObject>
#include<QDebug>
#include<QThread>
#include <QWidget>
#include <QDateTime>
#include <QTimer>

class OPENVIO;
#include "openvio.h"

class CamProcess:public QObject        
{
    Q_OBJECT
private:
    OPENVIO *vio = NULL;    
    Qt::CheckState showFlag = Qt::CheckState::Unchecked;
public:
    CamProcess(OPENVIO *vio,QObject* parent = nullptr);
    void cvProcess(QImage qImage);
    void setShowFlag(Qt::CheckState flag);
public slots:
    void camSlot(int index);

signals:
    void visionImageSignals(QPixmap qImage);  
};
 
#endif // CAMPROCESS_H