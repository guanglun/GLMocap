#include "PX4Thread.h"

#include <sys/time.h> //clock, gettimeofday, time

#define SYSTEM_ID 1
#define COMPONENT_ID 1

static uint64_t microsSinceEpoch();

PX4Thread::PX4Thread()
{
    m_pUdpServer = new QUdpSocket();
    connect(m_pUdpServer, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    m_pUdpServer->bind(14550);
}

void PX4Thread::run()
{
    uint8_t buf[BUFFER_LENGTH];
    int bytes_sent;
    mavlink_message_t msg;
    uint16_t len;

    float x = 10, y = 21, z = 13;
    float roll, pitch, yaw;

    setInterval(MAVLINK_MSG_ID_VISION_POSITION_ESTIMATE, 1000000);
    setInterval(MAVLINK_MSG_ID_ATTITUDE, 100000);

    for (;;)
    {

        float covariance[21] = {NAN};
        //uint8_t reset_counter = 10;
        mavlink_msg_vision_position_estimate_pack(1, 1, &msg,
                                                  microsSinceEpoch(),
                                                  x, y, z,
                                                  roll, pitch, yaw,
                                                  covariance, 0);
        len = mavlink_msg_to_send_buffer(buf, &msg);
        bytes_sent = m_pUdpServer->writeDatagram((const char *)buf, len, QHostAddress("192.168.2.1"), 14550);

        QThread::msleep(100);
    }
}

void PX4Thread::dataReceived()
{
    while (m_pUdpServer->hasPendingDatagrams()) //有数据报可读
    {
        QByteArray datagram;
        datagram.resize(m_pUdpServer->pendingDatagramSize());
        m_pUdpServer->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        // QString strMes(datagram);
        // qDebug() << strMes;
        mavlink_message_t msg;
        mavlink_status_t status;
        for (int i = 0; i < datagram.size(); i++)
        {
            if (mavlink_parse_char(MAVLINK_COMM_0, datagram.data()[i], &msg, &status))
            {
                // Packet received
                //printf("Received packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\r\n", msg.sysid, msg.compid, msg.len, msg.msgid);
                //printf(".");
                switch (msg.msgid)
                {
                case MAVLINK_MSG_ID_ODOMETRY:
                    printf("Received packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\r\n", msg.sysid, msg.compid, msg.len, msg.msgid);
                    break;
                default:
                    break;
                }
            }
        }
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

void PX4Thread::setInterval(int id, int interval_us)
{
    mavlink_command_long_t interval;

    interval.param1 = id;
    interval.param2 = interval_us;
    interval.command = MAV_CMD_SET_MESSAGE_INTERVAL;
    interval.target_system = 1;
    interval.target_component = 1;

    mavlink_msg_command_long_encode(255, 0, &msg, &interval);

    len = mavlink_msg_to_send_buffer(buf, &msg);
    bytes_sent = m_pUdpServer->writeDatagram((const char *)buf, len, QHostAddress("192.168.2.1"), 14550);
}

/* Ctrl Success
        mavlink_command_long_t interval;

        interval.param1 = MAVLINK_MSG_ID_ATTITUDE;
        interval.param2 = 1000000;
        interval.command = MAV_CMD_SET_MESSAGE_INTERVAL;
        interval.target_system = 1;
        interval.target_component = 1;

        mavlink_msg_command_long_encode(255, 0, &msg, &interval);

        len = mavlink_msg_to_send_buffer(buf, &msg);
        bytes_sent = m_pUdpServer->writeDatagram((const char *)buf, len, QHostAddress("192.168.2.1"), 14550);

        //Send Heartbeat
        // mavlink_msg_heartbeat_pack(SYSTEM_ID, COMPONENT_ID, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_PX4, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
        // len = mavlink_msg_to_send_buffer(buf, &msg);
        // bytes_sent = m_pUdpServer->writeDatagram((const char *)buf, len, QHostAddress("192.168.2.1"), 14551);

*/
