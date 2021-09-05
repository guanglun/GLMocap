#ifndef PX4THREAD_H
#define PX4THREAD_H

#include <QThread>
#include <QTimer>
#include "workspace.h"
#include <QUdpSocket>
#include <mavlink.h>


#define BUFFER_LENGTH 2041 // minimum buffer size that can be used with qnx (I don't know why)

class PX4Thread : public QThread
{
    Q_OBJECT
private:
    QUdpSocket *m_pUdpServer;
    QHostAddress sender;
    quint16 senderPort;


    float x = 0, y = 0, z = 0;
    float roll = 0, pitch = 0, yaw = 0;

    
    QTimer *timer;

public:
    float tar_x = 0, tar_y = 0, tar_z = 0,tar_yaw = 0;

    PX4Thread();
    void run();
    void setPos(float x, float y, float z, float roll, float pitch, float yaw);
    void setInterval(int id,int interval_us);
    void initMavlink(void);
    void sendTargetPosition(float x,float y,float z,float yaw);
    void setTargetPosition(float x,float y,float z,float yaw);
    void setArm(int arm);
signals:

private slots:
    void onTimeOut(); 
};

#endif
