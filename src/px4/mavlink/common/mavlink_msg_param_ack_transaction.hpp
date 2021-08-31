// MESSAGE PARAM_ACK_TRANSACTION support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief PARAM_ACK_TRANSACTION message
 *
 * Response from a PARAM_SET message when it is used in a transaction.
 */
struct PARAM_ACK_TRANSACTION : mavlink::Message {
    static constexpr msgid_t MSG_ID = 19;
    static constexpr size_t LENGTH = 24;
    static constexpr size_t MIN_LENGTH = 24;
    static constexpr uint8_t CRC_EXTRA = 137;
    static constexpr auto NAME = "PARAM_ACK_TRANSACTION";


    uint8_t target_system; /*<  Id of system that sent PARAM_SET message. */
    uint8_t target_component; /*<  Id of system that sent PARAM_SET message. */
    std::array<char, 16> param_id; /*<  Parameter id, terminated by NULL if the length is less than 16 human-readable chars and WITHOUT null termination (NULL) byte if the length is exactly 16 chars - applications have to provide 16+1 bytes storage if the ID is stored as string */
    float param_value; /*<  Parameter value (new value if PARAM_ACCEPTED, current value otherwise) */
    uint8_t param_type; /*<  Parameter type. */
    uint8_t param_result; /*<  Result code. */


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
        ss << "  param_id: \"" << to_string(param_id) << "\"" << std::endl;
        ss << "  param_value: " << param_value << std::endl;
        ss << "  param_type: " << +param_type << std::endl;
        ss << "  param_result: " << +param_result << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << param_value;                   // offset: 0
        map << target_system;                 // offset: 4
        map << target_component;              // offset: 5
        map << param_id;                      // offset: 6
        map << param_type;                    // offset: 22
        map << param_result;                  // offset: 23
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> param_value;                   // offset: 0
        map >> target_system;                 // offset: 4
        map >> target_component;              // offset: 5
        map >> param_id;                      // offset: 6
        map >> param_type;                    // offset: 22
        map >> param_result;                  // offset: 23
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
