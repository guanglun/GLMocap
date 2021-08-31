// MESSAGE SUPPORTED_TUNES support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief SUPPORTED_TUNES message
 *
 * Tune formats supported by vehicle. This should be emitted as response to MAV_CMD_REQUEST_MESSAGE.
 */
struct SUPPORTED_TUNES : mavlink::Message {
    static constexpr msgid_t MSG_ID = 401;
    static constexpr size_t LENGTH = 6;
    static constexpr size_t MIN_LENGTH = 6;
    static constexpr uint8_t CRC_EXTRA = 183;
    static constexpr auto NAME = "SUPPORTED_TUNES";


    uint8_t target_system; /*<  System ID */
    uint8_t target_component; /*<  Component ID */
    uint32_t format; /*<  Bitfield of supported tune formats. */


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
        ss << "  format: " << format << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << format;                        // offset: 0
        map << target_system;                 // offset: 4
        map << target_component;              // offset: 5
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> format;                        // offset: 0
        map >> target_system;                 // offset: 4
        map >> target_component;              // offset: 5
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
