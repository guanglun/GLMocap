// MESSAGE GIMBAL_DEVICE_SET_ATTITUDE support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief GIMBAL_DEVICE_SET_ATTITUDE message
 *
 * Low level message to control a gimbal device's attitude. This message is to be sent from the gimbal manager to the gimbal device component. Angles and rates can be set to NaN according to use case.
 */
struct GIMBAL_DEVICE_SET_ATTITUDE : mavlink::Message {
    static constexpr msgid_t MSG_ID = 284;
    static constexpr size_t LENGTH = 32;
    static constexpr size_t MIN_LENGTH = 32;
    static constexpr uint8_t CRC_EXTRA = 99;
    static constexpr auto NAME = "GIMBAL_DEVICE_SET_ATTITUDE";


    uint8_t target_system; /*<  System ID */
    uint8_t target_component; /*<  Component ID */
    uint16_t flags; /*<  Low level gimbal flags. */
    std::array<float, 4> q; /*<  Quaternion components, w, x, y, z (1 0 0 0 is the null-rotation, the frame is depends on whether the flag GIMBAL_DEVICE_FLAGS_YAW_LOCK is set, set all fields to NaN if only angular velocity should be used) */
    float angular_velocity_x; /*< [rad/s] X component of angular velocity, positive is rolling to the right, NaN to be ignored. */
    float angular_velocity_y; /*< [rad/s] Y component of angular velocity, positive is pitching up, NaN to be ignored. */
    float angular_velocity_z; /*< [rad/s] Z component of angular velocity, positive is yawing to the right, NaN to be ignored. */


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
        ss << "  target_system: " << +target_system << std::endl;
        ss << "  target_component: " << +target_component << std::endl;
        ss << "  flags: " << flags << std::endl;
        ss << "  q: [" << to_string(q) << "]" << std::endl;
        ss << "  angular_velocity_x: " << angular_velocity_x << std::endl;
        ss << "  angular_velocity_y: " << angular_velocity_y << std::endl;
        ss << "  angular_velocity_z: " << angular_velocity_z << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << q;                             // offset: 0
        map << angular_velocity_x;            // offset: 16
        map << angular_velocity_y;            // offset: 20
        map << angular_velocity_z;            // offset: 24
        map << flags;                         // offset: 28
        map << target_system;                 // offset: 30
        map << target_component;              // offset: 31
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> q;                             // offset: 0
        map >> angular_velocity_x;            // offset: 16
        map >> angular_velocity_y;            // offset: 20
        map >> angular_velocity_z;            // offset: 24
        map >> flags;                         // offset: 28
        map >> target_system;                 // offset: 30
        map >> target_component;              // offset: 31
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
