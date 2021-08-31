#include "PX4Thread.h"
#include <mavlink.h>
#include <QUdpSocket>
#include <sys/time.h> //clock, gettimeofday, time

#define BUFFER_LENGTH 2041 // minimum buffer size that can be used with qnx (I don't know why)

#define SYSTEM_ID 1
#define COMPONENT_ID 200

static uint64_t microsSinceEpoch();

PX4Thread::PX4Thread()
{
}

void PX4Thread::run()
{
    uint8_t buf[BUFFER_LENGTH];
    int bytes_sent;
    mavlink_message_t msg;
    uint16_t len;

    float x, y, z;
    float roll, pitch, yaw;

    QHostAddress sender;
    quint16 senderPort;
    QUdpSocket *m_pUdpServer = new QUdpSocket();
    m_pUdpServer->bind(QHostAddress::LocalHost, 7755);

    for (;;)
    {
        /*Send Heartbeat */
        mavlink_msg_heartbeat_pack(SYSTEM_ID, COMPONENT_ID, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_PX4, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
        len = mavlink_msg_to_send_buffer(buf, &msg);
        bytes_sent = m_pUdpServer->writeDatagram((const char *)buf, len, QHostAddress::Broadcast, 8086);

        float covariance[21] = {NAN};
        uint8_t reset_counter = 10;
        mavlink_msg_vision_position_estimate_pack(SYSTEM_ID, COMPONENT_ID, &msg,
                                                  microsSinceEpoch(),
                                                  x, y, z,
                                                  roll, pitch, yaw,
                                                  covariance, reset_counter);
        len = mavlink_msg_to_send_buffer(buf, &msg);
        bytes_sent = m_pUdpServer->writeDatagram((const char *)buf, len, QHostAddress::Broadcast, 8086);

        if (m_pUdpServer->hasPendingDatagrams())
        {
            QByteArray datagram;
            datagram.resize(m_pUdpServer->pendingDatagramSize());
            m_pUdpServer->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

            QString strMes(datagram);
            qDebug() << strMes;
			mavlink_message_t msg;
			mavlink_status_t status;
            for(int i=0;i<datagram.size();i++)
            {
				if (mavlink_parse_char(MAVLINK_COMM_0, datagram.data()[i], &msg, &status))
				{
					// Packet received
					printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
				}
            }

        }

        QThread::msleep(1000);
    }
}

static uint64_t microsSinceEpoch()
{

    struct timeval tv;

    uint64_t micros = 0;

    gettimeofday(&tv, NULL);
    micros = ((uint64_t)tv.tv_sec) * 1000000 + tv.tv_usec;

    return micros;
}
