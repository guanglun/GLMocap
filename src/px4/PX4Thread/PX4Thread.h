#ifndef PX4THREAD_H
#define PX4THREAD_H

#include <QThread>
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

    uint8_t buf[BUFFER_LENGTH];
    int bytes_sent;
    mavlink_message_t msg;
    uint16_t len;

public:
    PX4Thread();
    void run();
    void setInterval(int id,int interval_us);
signals:

private slots:
    void dataReceived();
};

#endif
