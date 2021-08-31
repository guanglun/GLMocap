// MESSAGE RESPONSE_EVENT_ERROR support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief RESPONSE_EVENT_ERROR message
 *
 * Response to a REQUEST_EVENT in case of an error (e.g. the event is not available anymore).
 */
struct RESPONSE_EVENT_ERROR : mavlink::Message {
    static constexpr msgid_t MSG_ID = 413;
    static constexpr size_t LENGTH = 7;
    static constexpr size_t MIN_LENGTH = 7;
    static constexpr uint8_t CRC_EXTRA = 77;
    static constexpr auto NAME = "RESPONSE_EVENT_ERROR";


    uint8_t target_system; /*<  System ID */
    uint8_t target_component; /*<  Component ID */
    uint16_t sequence; /*<  Sequence number. */
    uint16_t sequence_oldest_available; /*<  Oldest Sequence number that is still available after the sequence set in REQUEST_EVENT. */
    uint8_t reason; /*<  Error reason. */


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
        ss << "  sequence: " << sequence << std::endl;
        ss << "  sequence_oldest_available: " << sequence_oldest_available << std::endl;
        ss << "  reason: " << +reason << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << sequence;                      // offset: 0
        map << sequence_oldest_available;     // offset: 2
        map << target_system;                 // offset: 4
        map << target_component;              // offset: 5
        map << reason;                        // offset: 6
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> sequence;                      // offset: 0
        map >> sequence_oldest_available;     // offset: 2
        map >> target_system;                 // offset: 4
        map >> target_component;              // offset: 5
        map >> reason;                        // offset: 6
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
