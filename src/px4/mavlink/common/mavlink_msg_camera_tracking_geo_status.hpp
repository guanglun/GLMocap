// MESSAGE CAMERA_TRACKING_GEO_STATUS support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief CAMERA_TRACKING_GEO_STATUS message
 *
 * Camera tracking status, sent while in active tracking. Use MAV_CMD_SET_MESSAGE_INTERVAL to define message interval.
 */
struct CAMERA_TRACKING_GEO_STATUS : mavlink::Message {
    static constexpr msgid_t MSG_ID = 276;
    static constexpr size_t LENGTH = 49;
    static constexpr size_t MIN_LENGTH = 49;
    static constexpr uint8_t CRC_EXTRA = 18;
    static constexpr auto NAME = "CAMERA_TRACKING_GEO_STATUS";


    uint8_t tracking_status; /*<  Current tracking status */
    int32_t lat; /*< [degE7] Latitude of tracked object */
    int32_t lon; /*< [degE7] Longitude of tracked object */
    float alt; /*< [m] Altitude of tracked object(AMSL, WGS84) */
    float h_acc; /*< [m] Horizontal accuracy. NAN if unknown */
    float v_acc; /*< [m] Vertical accuracy. NAN if unknown */
    float vel_n; /*< [m/s] North velocity of tracked object. NAN if unknown */
    float vel_e; /*< [m/s] East velocity of tracked object. NAN if unknown */
    float vel_d; /*< [m/s] Down velocity of tracked object. NAN if unknown */
    float vel_acc; /*< [m/s] Velocity accuracy. NAN if unknown */
    float dist; /*< [m] Distance between camera and tracked object. NAN if unknown */
    float hdg; /*< [rad] Heading in radians, in NED. NAN if unknown */
    float hdg_acc; /*< [rad] Accuracy of heading, in NED. NAN if unknown */


    inline std::string get_name(void) const override
    {
            return NAME;
    }

    inline Info get_message_info(void) const override
    {
            return { MSG_ID, LENGTH, MIN_LENGTH, CRC_EXTRA };
    }

    inline std::string to_yaml(void) const override
    {
        std::stringstream ss;

        ss << NAME << ":" << std::endl;
        ss << "  tracking_status: " << +tracking_status << std::endl;
        ss << "  lat: " << lat << std::endl;
        ss << "  lon: " << lon << std::endl;
        ss << "  alt: " << alt << std::endl;
        ss << "  h_acc: " << h_acc << std::endl;
        ss << "  v_acc: " << v_acc << std::endl;
        ss << "  vel_n: " << vel_n << std::endl;
        ss << "  vel_e: " << vel_e << std::endl;
        ss << "  vel_d: " << vel_d << std::endl;
        ss << "  vel_acc: " << vel_acc << std::endl;
        ss << "  dist: " << dist << std::endl;
        ss << "  hdg: " << hdg << std::endl;
        ss << "  hdg_acc: " << hdg_acc << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << lat;                           // offset: 0
        map << lon;                           // offset: 4
        map << alt;                           // offset: 8
        map << h_acc;                         // offset: 12
        map << v_acc;                         // offset: 16
        map << vel_n;                         // offset: 20
        map << vel_e;                         // offset: 24
        map << vel_d;                         // offset: 28
        map << vel_acc;                       // offset: 32
        map << dist;                          // offset: 36
        map << hdg;                           // offset: 40
        map << hdg_acc;                       // offset: 44
        map << tracking_status;               // offset: 48
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> lat;                           // offset: 0
        map >> lon;                           // offset: 4
        map >> alt;                           // offset: 8
        map >> h_acc;                         // offset: 12
        map >> v_acc;                         // offset: 16
        map >> vel_n;                         // offset: 20
        map >> vel_e;                         // offset: 24
        map >> vel_d;                         // offset: 28
        map >> vel_acc;                       // offset: 32
        map >> dist;                          // offset: 36
        map >> hdg;                           // offset: 40
        map >> hdg_acc;                       // offset: 44
        map >> tracking_status;               // offset: 48
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
