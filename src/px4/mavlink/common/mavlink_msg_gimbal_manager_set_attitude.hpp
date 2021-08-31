// MESSAGE GIMBAL_MANAGER_SET_ATTITUDE support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief GIMBAL_MANAGER_SET_ATTITUDE message
 *
 * High level message to control a gimbal's attitude. This message is to be sent to the gimbal manager (e.g. from a ground station). Angles and rates can be set to NaN according to use case.
 */
struct GIMBAL_MANAGER_SET_ATTITUDE : mavlink::Message {
    static constexpr msgid_t MSG_ID = 282;
    static constexpr size_t LENGTH = 35;
    static constexpr size_t MIN_LENGTH = 35;
    static constexpr uint8_t CRC_EXTRA = 123;
    static constexpr auto NAME = "GIMBAL_MANAGER_SET_ATTITUDE";


    uint8_t target_system; /*<  System ID */
    uint8_t target_component; /*<  Component ID */
    uint32_t flags; /*<  High level gimbal manager flags to use. */
    uint8_t gimbal_device_id; /*<  Component ID of gimbal device to address (or 1-6 for non-MAVLink gimbal), 0 for all gimbal device components. Send command multiple times for more than one gimbal (but not all gimbals). */
    std::array<float, 4> q; /*<  Quaternion components, w, x, y, z (1 0 0 0 is the null-rotation, the frame is depends on whether the flag GIMBAL_MANAGER_FLAGS_YAW_LOCK is set) */
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
        ss << "  gimbal_device_id: " << +gimbal_device_id << std::endl;
        ss << "  q: [" << to_string(q) << "]" << std::endl;
        ss << "  angular_velocity_x: " << angular_velocity_x << std::endl;
        ss << "  angular_velocity_y: " << angular_velocity_y << std::endl;
        ss << "  angular_velocity_z: " << angular_velocity_z << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << flags;                         // offset: 0
        map << q;                             // offset: 4
        map << angular_velocity_x;            // offset: 20
        map << angular_velocity_y;            // offset: 24
        map << angular_velocity_z;            // offset: 28
        map << target_system;                 // offset: 32
        map << target_component;              // offset: 33
        map << gimbal_device_id;              // offset: 34
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> flags;                         // offset: 0
        map >> q;                             // offset: 4
        map >> angular_velocity_x;            // offset: 20
        map >> angular_velocity_y;            // offset: 24
        map >> angular_velocity_z;            // offset: 28
        map >> target_system;                 // offset: 32
        map >> target_component;              // offset: 33
        map >> gimbal_device_id;              // offset: 34
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
