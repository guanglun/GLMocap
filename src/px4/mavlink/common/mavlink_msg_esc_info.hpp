// MESSAGE ESC_INFO support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief ESC_INFO message
 *
 * ESC information for lower rate streaming. Recommended streaming rate 1Hz. See ESC_STATUS for higher-rate ESC data.
 */
struct ESC_INFO : mavlink::Message {
    static constexpr msgid_t MSG_ID = 290;
    static constexpr size_t LENGTH = 46;
    static constexpr size_t MIN_LENGTH = 46;
    static constexpr uint8_t CRC_EXTRA = 251;
    static constexpr auto NAME = "ESC_INFO";


    uint8_t index; /*<  Index of the first ESC in this message. minValue = 0, maxValue = 60, increment = 4. */
    uint64_t time_usec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude the number. */
    uint16_t counter; /*<  Counter of data packets received. */
    uint8_t count; /*<  Total number of ESCs in all messages of this type. Message fields with an index higher than this should be ignored because they contain invalid data. */
    uint8_t connection_type; /*<  Connection type protocol for all ESC. */
    uint8_t info; /*<  Information regarding online/offline status of each ESC. */
    std::array<uint16_t, 4> failure_flags; /*<  Bitmap of ESC failure flags. */
    std::array<uint32_t, 4> error_count; /*<  Number of reported errors by each ESC since boot. */
    std::array<int16_t, 4> temperature; /*< [cdegC] Temperature of each ESC. INT16_MAX: if data not supplied by ESC. */


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
        ss << "  index: " << +index << std::endl;
        ss << "  time_usec: " << time_usec << std::endl;
        ss << "  counter: " << counter << std::endl;
        ss << "  count: " << +count << std::endl;
        ss << "  connection_type: " << +connection_type << std::endl;
        ss << "  info: " << +info << std::endl;
        ss << "  failure_flags: [" << to_string(failure_flags) << "]" << std::endl;
        ss << "  error_count: [" << to_string(error_count) << "]" << std::endl;
        ss << "  temperature: [" << to_string(temperature) << "]" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << time_usec;                     // offset: 0
        map << error_count;                   // offset: 8
        map << counter;                       // offset: 24
        map << failure_flags;                 // offset: 26
        map << temperature;                   // offset: 34
        map << index;                         // offset: 42
        map << count;                         // offset: 43
        map << connection_type;               // offset: 44
        map << info;                          // offset: 45
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> time_usec;                     // offset: 0
        map >> error_count;                   // offset: 8
        map >> counter;                       // offset: 24
        map >> failure_flags;                 // offset: 26
        map >> temperature;                   // offset: 34
        map >> index;                         // offset: 42
        map >> count;                         // offset: 43
        map >> connection_type;               // offset: 44
        map >> info;                          // offset: 45
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
