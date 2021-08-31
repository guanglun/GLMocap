// MESSAGE CURRENT_EVENT_SEQUENCE support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief CURRENT_EVENT_SEQUENCE message
 *
 * Regular broadcast for the current latest event sequence number for a component. This is used to check for dropped events.
 */
struct CURRENT_EVENT_SEQUENCE : mavlink::Message {
    static constexpr msgid_t MSG_ID = 411;
    static constexpr size_t LENGTH = 3;
    static constexpr size_t MIN_LENGTH = 3;
    static constexpr uint8_t CRC_EXTRA = 106;
    static constexpr auto NAME = "CURRENT_EVENT_SEQUENCE";


    uint16_t sequence; /*<  Sequence number. */
    uint8_t flags; /*<  Flag bitset. */


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
        ss << "  sequence: " << sequence << std::endl;
        ss << "  flags: " << +flags << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << sequence;                      // offset: 0
        map << flags;                         // offset: 2
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> sequence;                      // offset: 0
        map >> flags;                         // offset: 2
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
