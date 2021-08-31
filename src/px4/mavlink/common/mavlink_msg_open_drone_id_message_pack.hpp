// MESSAGE OPEN_DRONE_ID_MESSAGE_PACK support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief OPEN_DRONE_ID_MESSAGE_PACK message
 *
 * An OpenDroneID message pack is a container for multiple encoded OpenDroneID messages (i.e. not in the format given for the above messages descriptions but after encoding into the compressed OpenDroneID byte format). Used e.g. when transmitting on Bluetooth 5.0 Long Range/Extended Advertising or on WiFi Neighbor Aware Networking.
 */
struct OPEN_DRONE_ID_MESSAGE_PACK : mavlink::Message {
    static constexpr msgid_t MSG_ID = 12915;
    static constexpr size_t LENGTH = 254;
    static constexpr size_t MIN_LENGTH = 254;
    static constexpr uint8_t CRC_EXTRA = 62;
    static constexpr auto NAME = "OPEN_DRONE_ID_MESSAGE_PACK";


    uint8_t target_system; /*<  System ID (0 for broadcast). */
    uint8_t target_component; /*<  Component ID (0 for broadcast). */
    uint8_t single_message_size; /*< [bytes] This field must currently always be equal to 25 (bytes), since all encoded OpenDroneID messages are specificed to have this length. */
    uint8_t msg_pack_size; /*<  Number of encoded messages in the pack (not the number of bytes). Allowed range is 1 - 10. */
    std::array<uint8_t, 250> messages; /*<  Concatenation of encoded OpenDroneID messages. Shall be filled with nulls in the unused portion of the field. */


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
        ss << "  single_message_size: " << +single_message_size << std::endl;
        ss << "  msg_pack_size: " << +msg_pack_size << std::endl;
        ss << "  messages: [" << to_string(messages) << "]" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << target_system;                 // offset: 0
        map << target_component;              // offset: 1
        map << single_message_size;           // offset: 2
        map << msg_pack_size;                 // offset: 3
        map << messages;                      // offset: 4
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> target_system;                 // offset: 0
        map >> target_component;              // offset: 1
        map >> single_message_size;           // offset: 2
        map >> msg_pack_size;                 // offset: 3
        map >> messages;                      // offset: 4
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
