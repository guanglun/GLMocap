// MESSAGE ESC_STATUS support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief ESC_STATUS message
 *
 * ESC information for higher rate streaming. Recommended streaming rate is ~10 Hz. Information that changes more slowly is sent in ESC_INFO. It should typically only be streamed on high-bandwidth links (i.e. to a companion computer).
 */
struct ESC_STATUS : mavlink::Message {
    static constexpr msgid_t MSG_ID = 291;
    static constexpr size_t LENGTH = 57;
    static constexpr size_t MIN_LENGTH = 57;
    static constexpr uint8_t CRC_EXTRA = 10;
    static constexpr auto NAME = "ESC_STATUS";


    uint8_t index; /*<  Index of the first ESC in this message. minValue = 0, maxValue = 60, increment = 4. */
    uint64_t time_usec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude the number. */
    std::array<int32_t, 4> rpm; /*< [rpm] Reported motor RPM from each ESC (negative for reverse rotation). */
    std::array<float, 4> voltage; /*< [V] Voltage measured from each ESC. */
    std::array<float, 4> current; /*< [A] Current measured from each ESC. */


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
        ss << "  rpm: [" << to_string(rpm) << "]" << std::endl;
        ss << "  voltage: [" << to_string(voltage) << "]" << std::endl;
        ss << "  current: [" << to_string(current) << "]" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << time_usec;                     // offset: 0
        map << rpm;                           // offset: 8
        map << voltage;                       // offset: 24
        map << current;                       // offset: 40
        map << index;                         // offset: 56
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> time_usec;                     // offset: 0
        map >> rpm;                           // offset: 8
        map >> voltage;                       // offset: 24
        map >> current;                       // offset: 40
        map >> index;                         // offset: 56
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
