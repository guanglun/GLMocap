// MESSAGE GIMBAL_MANAGER_INFORMATION support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief GIMBAL_MANAGER_INFORMATION message
 *
 * Information about a high level gimbal manager. This message should be requested by a ground station using MAV_CMD_REQUEST_MESSAGE.
 */
struct GIMBAL_MANAGER_INFORMATION : mavlink::Message {
    static constexpr msgid_t MSG_ID = 280;
    static constexpr size_t LENGTH = 33;
    static constexpr size_t MIN_LENGTH = 33;
    static constexpr uint8_t CRC_EXTRA = 70;
    static constexpr auto NAME = "GIMBAL_MANAGER_INFORMATION";


    uint32_t time_boot_ms; /*< [ms] Timestamp (time since system boot). */
    uint32_t cap_flags; /*<  Bitmap of gimbal capability flags. */
    uint8_t gimbal_device_id; /*<  Gimbal device ID that this gimbal manager is responsible for. */
    float roll_min; /*< [rad] Minimum hardware roll angle (positive: rolling to the right, negative: rolling to the left) */
    float roll_max; /*< [rad] Maximum hardware roll angle (positive: rolling to the right, negative: rolling to the left) */
    float pitch_min; /*< [rad] Minimum pitch angle (positive: up, negative: down) */
    float pitch_max; /*< [rad] Maximum pitch angle (positive: up, negative: down) */
    float yaw_min; /*< [rad] Minimum yaw angle (positive: to the right, negative: to the left) */
    float yaw_max; /*< [rad] Maximum yaw angle (positive: to the right, negative: to the left) */


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
        ss << "  time_boot_ms: " << time_boot_ms << std::endl;
        ss << "  cap_flags: " << cap_flags << std::endl;
        ss << "  gimbal_device_id: " << +gimbal_device_id << std::endl;
        ss << "  roll_min: " << roll_min << std::endl;
        ss << "  roll_max: " << roll_max << std::endl;
        ss << "  pitch_min: " << pitch_min << std::endl;
        ss << "  pitch_max: " << pitch_max << std::endl;
        ss << "  yaw_min: " << yaw_min << std::endl;
        ss << "  yaw_max: " << yaw_max << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << time_boot_ms;                  // offset: 0
        map << cap_flags;                     // offset: 4
        map << roll_min;                      // offset: 8
        map << roll_max;                      // offset: 12
        map << pitch_min;                     // offset: 16
        map << pitch_max;                     // offset: 20
        map << yaw_min;                       // offset: 24
        map << yaw_max;                       // offset: 28
        map << gimbal_device_id;              // offset: 32
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> time_boot_ms;                  // offset: 0
        map >> cap_flags;                     // offset: 4
        map >> roll_min;                      // offset: 8
        map >> roll_max;                      // offset: 12
        map >> pitch_min;                     // offset: 16
        map >> pitch_max;                     // offset: 20
        map >> yaw_min;                       // offset: 24
        map >> yaw_max;                       // offset: 28
        map >> gimbal_device_id;              // offset: 32
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
