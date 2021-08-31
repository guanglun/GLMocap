// MESSAGE GIMBAL_DEVICE_ATTITUDE_STATUS support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief GIMBAL_DEVICE_ATTITUDE_STATUS message
 *
 * Message reporting the status of a gimbal device. This message should be broadcasted by a gimbal device component. The angles encoded in the quaternion are in the global frame (roll: positive is rolling to the right, pitch: positive is pitching up, yaw is turn to the right). This message should be broadcast at a low regular rate (e.g. 10Hz).
 */
struct GIMBAL_DEVICE_ATTITUDE_STATUS : mavlink::Message {
    static constexpr msgid_t MSG_ID = 285;
    static constexpr size_t LENGTH = 40;
    static constexpr size_t MIN_LENGTH = 40;
    static constexpr uint8_t CRC_EXTRA = 137;
    static constexpr auto NAME = "GIMBAL_DEVICE_ATTITUDE_STATUS";


    uint8_t target_system; /*<  System ID */
    uint8_t target_component; /*<  Component ID */
    uint32_t time_boot_ms; /*< [ms] Timestamp (time since system boot). */
    uint16_t flags; /*<  Current gimbal flags set. */
    std::array<float, 4> q; /*<  Quaternion components, w, x, y, z (1 0 0 0 is the null-rotation, the frame is depends on whether the flag GIMBAL_DEVICE_FLAGS_YAW_LOCK is set) */
    float angular_velocity_x; /*< [rad/s] X component of angular velocity (NaN if unknown) */
    float angular_velocity_y; /*< [rad/s] Y component of angular velocity (NaN if unknown) */
    float angular_velocity_z; /*< [rad/s] Z component of angular velocity (NaN if unknown) */
    uint32_t failure_flags; /*<  Failure flags (0 for no failure) */


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
        ss << "  time_boot_ms: " << time_boot_ms << std::endl;
        ss << "  flags: " << flags << std::endl;
        ss << "  q: [" << to_string(q) << "]" << std::endl;
        ss << "  angular_velocity_x: " << angular_velocity_x << std::endl;
        ss << "  angular_velocity_y: " << angular_velocity_y << std::endl;
        ss << "  angular_velocity_z: " << angular_velocity_z << std::endl;
        ss << "  failure_flags: " << failure_flags << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << time_boot_ms;                  // offset: 0
        map << q;                             // offset: 4
        map << angular_velocity_x;            // offset: 20
        map << angular_velocity_y;            // offset: 24
        map << angular_velocity_z;            // offset: 28
        map << failure_flags;                 // offset: 32
        map << flags;                         // offset: 36
        map << target_system;                 // offset: 38
        map << target_component;              // offset: 39
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> time_boot_ms;                  // offset: 0
        map >> q;                             // offset: 4
        map >> angular_velocity_x;            // offset: 20
        map >> angular_velocity_y;            // offset: 24
        map >> angular_velocity_z;            // offset: 28
        map >> failure_flags;                 // offset: 32
        map >> flags;                         // offset: 36
        map >> target_system;                 // offset: 38
        map >> target_component;              // offset: 39
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
