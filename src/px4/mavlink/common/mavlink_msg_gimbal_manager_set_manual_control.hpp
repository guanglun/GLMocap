// MESSAGE GIMBAL_MANAGER_SET_MANUAL_CONTROL support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief GIMBAL_MANAGER_SET_MANUAL_CONTROL message
 *
 * High level message to control a gimbal manually. The angles or angular rates are unitless; the actual rates will depend on internal gimbal manager settings/configuration (e.g. set by parameters). This message is to be sent to the gimbal manager (e.g. from a ground station). Angles and rates can be set to NaN according to use case.
 */
struct GIMBAL_MANAGER_SET_MANUAL_CONTROL : mavlink::Message {
    static constexpr msgid_t MSG_ID = 288;
    static constexpr size_t LENGTH = 23;
    static constexpr size_t MIN_LENGTH = 23;
    static constexpr uint8_t CRC_EXTRA = 20;
    static constexpr auto NAME = "GIMBAL_MANAGER_SET_MANUAL_CONTROL";


    uint8_t target_system; /*<  System ID */
    uint8_t target_component; /*<  Component ID */
    uint32_t flags; /*<  High level gimbal manager flags. */
    uint8_t gimbal_device_id; /*<  Component ID of gimbal device to address (or 1-6 for non-MAVLink gimbal), 0 for all gimbal device components. Send command multiple times for more than one gimbal (but not all gimbals). */
    float pitch; /*<  Pitch angle unitless (-1..1, positive: up, negative: down, NaN to be ignored). */
    float yaw; /*<  Yaw angle unitless (-1..1, positive: to the right, negative: to the left, NaN to be ignored). */
    float pitch_rate; /*<  Pitch angular rate unitless (-1..1, positive: up, negative: down, NaN to be ignored). */
    float yaw_rate; /*<  Yaw angular rate unitless (-1..1, positive: to the right, negative: to the left, NaN to be ignored). */


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
        ss << "  pitch: " << pitch << std::endl;
        ss << "  yaw: " << yaw << std::endl;
        ss << "  pitch_rate: " << pitch_rate << std::endl;
        ss << "  yaw_rate: " << yaw_rate << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << flags;                         // offset: 0
        map << pitch;                         // offset: 4
        map << yaw;                           // offset: 8
        map << pitch_rate;                    // offset: 12
        map << yaw_rate;                      // offset: 16
        map << target_system;                 // offset: 20
        map << target_component;              // offset: 21
        map << gimbal_device_id;              // offset: 22
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> flags;                         // offset: 0
        map >> pitch;                         // offset: 4
        map >> yaw;                           // offset: 8
        map >> pitch_rate;                    // offset: 12
        map >> yaw_rate;                      // offset: 16
        map >> target_system;                 // offset: 20
        map >> target_component;              // offset: 21
        map >> gimbal_device_id;              // offset: 22
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
