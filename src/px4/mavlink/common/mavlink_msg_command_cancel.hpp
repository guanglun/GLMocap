// MESSAGE COMMAND_CANCEL support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief COMMAND_CANCEL message
 *
 * Cancel a long running command. The target system should respond with a COMMAND_ACK to the original command with result=MAV_RESULT_CANCELLED if the long running process was cancelled. If it has already completed, the cancel action can be ignored. The cancel action can be retried until some sort of acknowledgement to the original command has been received. The command microservice is documented at https://mavlink.io/en/services/command.html
 */
struct COMMAND_CANCEL : mavlink::Message {
    static constexpr msgid_t MSG_ID = 80;
    static constexpr size_t LENGTH = 4;
    static constexpr size_t MIN_LENGTH = 4;
    static constexpr uint8_t CRC_EXTRA = 14;
    static constexpr auto NAME = "COMMAND_CANCEL";


    uint8_t target_system; /*<  System executing long running command. Should not be broadcast (0). */
    uint8_t target_component; /*<  Component executing long running command. */
    uint16_t command; /*<  Command ID (of command to cancel). */


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
        ss << "  command: " << command << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << command;                       // offset: 0
        map << target_system;                 // offset: 2
        map << target_component;              // offset: 3
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> command;                       // offset: 0
        map >> target_system;                 // offset: 2
        map >> target_component;              // offset: 3
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
