#include "PX4Thread.h"

#include <sys/time.h> //clock, gettimeofday, time

#define SYSTEM_ID 1
#define COMPONENT_ID 1

//#define TARGET_IP "192.168.137.18"
#define TARGET_IP "192.168.2.1"
#define TARGET_PORT 14550
static uint64_t microsSinceEpoch();

PX4Thread::PX4Thread()
{
    m_pUdpServer = new QUdpSocket();
    m_pUdpServer->bind(TARGET_PORT);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer->start(100);
}

void PX4Thread::run()
{
    uint8_t buf[BUFFER_LENGTH];
    int bytes_sent;
    mavlink_message_t msg;
    uint16_t len;

    for (;;)
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
                    case MAVLINK_MSG_ID_ODOMETRY: //#331
                        printf("Received packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\r\n", msg.sysid, msg.compid, msg.len, msg.msgid);

                        mavlink_odometry_t odometry;
                        mavlink_msg_odometry_decode(&msg, &odometry);

                        printf("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\r\n",
                               odometry.x, odometry.y, odometry.z,
                               odometry.q[0], odometry.q[1], odometry.q[2], odometry.q[3]);

                        break;
                    default:
                        break;
                    }
                }
            }
        }

        //QThread::msleep(1);
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

void PX4Thread::onTimeOut()
{
    // QString str = "position: " +
    //                     QString::number(tar_x, 'f', 2) + "  " +
    //                     QString::number(tar_y, 'f', 2) + "  " +
    //                     QString::number(tar_z, 'f', 2) + "  " +
    //                     QString::number(tar_yaw, 'f', 2);

    // mlog->show(str);

    sendTargetPosition(tar_x, tar_y, tar_z, tar_yaw);
}

void PX4Thread::setPos(float x, float y, float z, float roll, float pitch, float yaw)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->roll = roll;
    this->pitch = pitch;
    this->yaw = yaw;

    mlog->show("pos : " +
               QString::number(this->x, 'f', 2) + "\t" +
               QString::number(this->y, 'f', 2) + "\t" +
               QString::number(this->z, 'f', 2) + "\t" +
               QString::number(this->roll, 'f', 2) + "\t" +
               QString::number(this->pitch, 'f', 2) + "\t" +
               QString::number(this->yaw, 'f', 2));

    uint8_t buf[BUFFER_LENGTH];
    int bytes_sent;
    mavlink_message_t msg;

    float covariance[21] = {NAN};
    //uint8_t reset_counter = 10;
    mavlink_msg_vision_position_estimate_pack(1, 1, &msg,
                                              microsSinceEpoch(),
                                              x, y, z,
                                              roll, pitch, yaw,
                                              covariance, 0);
    int len = mavlink_msg_to_send_buffer(buf, &msg);
    bytes_sent = m_pUdpServer->writeDatagram((const char *)buf, len, QHostAddress(TARGET_IP), TARGET_PORT);
}

void PX4Thread::initMavlink(void)
{
    setInterval(MAVLINK_MSG_ID_ODOMETRY, 20000);
    setInterval(MAVLINK_MSG_ID_ATTITUDE, 100000);
    setInterval(MAVLINK_MSG_ID_POSITION_TARGET_LOCAL_NED, 100000);
}

void PX4Thread::setTargetPosition(float x, float y, float z, float yaw)
{
    tar_x = x;
    tar_y = y;
    tar_z = z;
    tar_yaw = yaw;

    //DBG("")
}

void PX4Thread::sendTargetPosition(float x, float y, float z, float yaw)
{
    uint8_t buf[BUFFER_LENGTH];
    int bytes_sent;
    mavlink_message_t msg;

    const uint16_t ignore = POSITION_TARGET_TYPEMASK_VX_IGNORE |
                            POSITION_TARGET_TYPEMASK_VY_IGNORE |
                            POSITION_TARGET_TYPEMASK_VZ_IGNORE |
                            POSITION_TARGET_TYPEMASK_AX_IGNORE |
                            POSITION_TARGET_TYPEMASK_AY_IGNORE |
                            POSITION_TARGET_TYPEMASK_AZ_IGNORE; //|
                                                                //POSITION_TARGET_TYPEMASK_FORCE_SET ;
                                                                //|POSITION_TARGET_TYPEMASK_YAW_RATE_IGNORE;

    mavlink_set_position_target_local_ned_t set_position_target_local_ned;
    set_position_target_local_ned.target_system = 0;
    set_position_target_local_ned.target_component = 0;
    set_position_target_local_ned.time_boot_ms = microsSinceEpoch();
    set_position_target_local_ned.coordinate_frame = MAV_FRAME_LOCAL_NED;
    set_position_target_local_ned.type_mask = ignore;
    set_position_target_local_ned.x = x;
    set_position_target_local_ned.y = -y;
    set_position_target_local_ned.z = -z;
    set_position_target_local_ned.yaw = yaw;
    set_position_target_local_ned.yaw_rate = 0;

    // QString str = "position: " +
    //               QString::number(set_position_target_local_ned.x, 'f', 2) + "  " +
    //               QString::number(set_position_target_local_ned.y, 'f', 2) + "  " +
    //               QString::number(set_position_target_local_ned.z, 'f', 2) + "  " +
    //               QString::number(set_position_target_local_ned.yaw, 'f', 2);

    // mlog->show(str);

    mavlink_msg_set_position_target_local_ned_encode(0, 0, &msg, &set_position_target_local_ned);
    int len = mavlink_msg_to_send_buffer(buf, &msg);
    m_pUdpServer->writeDatagram((const char *)buf, len, QHostAddress(TARGET_IP), TARGET_PORT);
}

void PX4Thread::setInterval(int id, int interval_us)
{
    uint8_t buf[BUFFER_LENGTH];
    int bytes_sent;
    mavlink_message_t msg;

    mavlink_command_long_t interval;

    interval.param1 = id;
    interval.param2 = interval_us;
    interval.command = MAV_CMD_SET_MESSAGE_INTERVAL;
    interval.target_system = 1;
    interval.target_component = 1;

    mavlink_msg_command_long_encode(255, 0, &msg, &interval);

    int len = mavlink_msg_to_send_buffer(buf, &msg);
    bytes_sent = m_pUdpServer->writeDatagram((const char *)buf, len, QHostAddress(TARGET_IP), TARGET_PORT);
}

void PX4Thread::setArm(int arm)
{
    uint8_t buf[BUFFER_LENGTH];
    int bytes_sent;
    mavlink_message_t msg;

    mavlink_command_long_t interval;

    interval.param1 = arm;
    interval.param2 = 21196;
    interval.command = MAV_CMD_COMPONENT_ARM_DISARM;
    interval.target_system = 1;
    interval.target_component = 1;

    mavlink_msg_command_long_encode(255, 0, &msg, &interval);

    int len = mavlink_msg_to_send_buffer(buf, &msg);
    bytes_sent = m_pUdpServer->writeDatagram((const char *)buf, len, QHostAddress(TARGET_IP), TARGET_PORT);
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
        bytes_sent = m_pUdpServer->writeDatagram((const char *)buf, len, QHostAddress("192.168.2.1"), TARGET_PORT);

        //Send Heartbeat
        // mavlink_msg_heartbeat_pack(SYSTEM_ID, COMPONENT_ID, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_PX4, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
        // len = mavlink_msg_to_send_buffer(buf, &msg);
        // bytes_sent = m_pUdpServer->writeDatagram((const char *)buf, len, QHostAddress("192.168.2.1"), 14551);

*/
