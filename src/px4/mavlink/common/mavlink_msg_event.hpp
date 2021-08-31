// MESSAGE EVENT support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief EVENT message
 *
 * Event message. Each new event from a particular component gets a new sequence number. The same message might be sent multiple times if (re-)requested. Most events are broadcast, some can be specific to a target component (as receivers keep track of the sequence for missed events, all events need to be broadcast. Thus we use destination_component instead of target_component).
 */
struct EVENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 410;
    static constexpr size_t LENGTH = 53;
    static constexpr size_t MIN_LENGTH = 53;
    static constexpr uint8_t CRC_EXTRA = 160;
    static constexpr auto NAME = "EVENT";


    uint8_t destination_component; /*<  Component ID */
    uint8_t destination_system; /*<  System ID */
    uint32_t id; /*<  Event ID (as defined in the component metadata) */
    uint32_t event_time_boot_ms; /*< [ms] Timestamp (time since system boot when the event happened). */
    uint16_t sequence; /*<  Sequence number. */
    uint8_t log_levels; /*<  Log levels: 4 bits MSB: internal (for logging purposes), 4 bits LSB: external. Levels: Emergency = 0, Alert = 1, Critical = 2, Error = 3, Warning = 4, Notice = 5, Info = 6, Debug = 7, Protocol = 8, Disabled = 9 */
    std::array<uint8_t, 40> arguments; /*<  Arguments (depend on event ID). */


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
        ss << "  destination_component: " << +destination_component << std::endl;
        ss << "  destination_system: " << +destination_system << std::endl;
        ss << "  id: " << id << std::endl;
        ss << "  event_time_boot_ms: " << event_time_boot_ms << std::endl;
        ss << "  sequence: " << sequence << std::endl;
        ss << "  log_levels: " << +log_levels << std::endl;
        ss << "  arguments: [" << to_string(arguments) << "]" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << id;                            // offset: 0
        map << event_time_boot_ms;            // offset: 4
        map << sequence;                      // offset: 8
        map << destination_component;         // offset: 10
        map << destination_system;            // offset: 11
        map << log_levels;                    // offset: 12
        map << arguments;                     // offset: 13
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> id;                            // offset: 0
        map >> event_time_boot_ms;            // offset: 4
        map >> sequence;                      // offset: 8
        map >> destination_component;         // offset: 10
        map >> destination_system;            // offset: 11
        map >> log_levels;                    // offset: 12
        map >> arguments;                     // offset: 13
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
