// MESSAGE GIMBAL_MANAGER_STATUS support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief GIMBAL_MANAGER_STATUS message
 *
 * Current status about a high level gimbal manager. This message should be broadcast at a low regular rate (e.g. 5Hz).
 */
struct GIMBAL_MANAGER_STATUS : mavlink::Message {
    static constexpr msgid_t MSG_ID = 281;
    static constexpr size_t LENGTH = 13;
    static constexpr size_t MIN_LENGTH = 13;
    static constexpr uint8_t CRC_EXTRA = 48;
    static constexpr auto NAME = "GIMBAL_MANAGER_STATUS";


    uint32_t time_boot_ms; /*< [ms] Timestamp (time since system boot). */
    uint32_t flags; /*<  High level gimbal manager flags currently applied. */
    uint8_t gimbal_device_id; /*<  Gimbal device ID that this gimbal manager is responsible for. */
    uint8_t primary_control_sysid; /*<  System ID of MAVLink component with primary control, 0 for none. */
    uint8_t primary_control_compid; /*<  Component ID of MAVLink component with primary control, 0 for none. */
    uint8_t secondary_control_sysid; /*<  System ID of MAVLink component with secondary control, 0 for none. */
    uint8_t secondary_control_compid; /*<  Component ID of MAVLink component with secondary control, 0 for none. */


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
        ss << "  flags: " << flags << std::endl;
        ss << "  gimbal_device_id: " << +gimbal_device_id << std::endl;
        ss << "  primary_control_sysid: " << +primary_control_sysid << std::endl;
        ss << "  primary_control_compid: " << +primary_control_compid << std::endl;
        ss << "  secondary_control_sysid: " << +secondary_control_sysid << std::endl;
        ss << "  secondary_control_compid: " << +secondary_control_compid << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << time_boot_ms;                  // offset: 0
        map << flags;                         // offset: 4
        map << gimbal_device_id;              // offset: 8
        map << primary_control_sysid;         // offset: 9
        map << primary_control_compid;        // offset: 10
        map << secondary_control_sysid;       // offset: 11
        map << secondary_control_compid;      // offset: 12
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> time_boot_ms;                  // offset: 0
        map >> flags;                         // offset: 4
        map >> gimbal_device_id;              // offset: 8
        map >> primary_control_sysid;         // offset: 9
        map >> primary_control_compid;        // offset: 10
        map >> secondary_control_sysid;       // offset: 11
        map >> secondary_control_compid;      // offset: 12
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
