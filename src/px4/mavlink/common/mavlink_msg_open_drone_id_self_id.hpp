// MESSAGE OPEN_DRONE_ID_SELF_ID support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief OPEN_DRONE_ID_SELF_ID message
 *
 * Data for filling the OpenDroneID Self ID message. The Self ID Message is an opportunity for the operator to (optionally) declare their identity and purpose of the flight. This message can provide additional information that could reduce the threat profile of a UA (Unmanned Aircraft) flying in a particular area or manner.
 */
struct OPEN_DRONE_ID_SELF_ID : mavlink::Message {
    static constexpr msgid_t MSG_ID = 12903;
    static constexpr size_t LENGTH = 46;
    static constexpr size_t MIN_LENGTH = 46;
    static constexpr uint8_t CRC_EXTRA = 249;
    static constexpr auto NAME = "OPEN_DRONE_ID_SELF_ID";


    uint8_t target_system; /*<  System ID (0 for broadcast). */
    uint8_t target_component; /*<  Component ID (0 for broadcast). */
    std::array<uint8_t, 20> id_or_mac; /*<  Only used for drone ID data received from other UAs. See detailed description at https://mavlink.io/en/services/opendroneid.html.  */
    uint8_t description_type; /*<  Indicates the type of the description field. */
    std::array<char, 23> description; /*<  Text description or numeric value expressed as ASCII characters. Shall be filled with nulls in the unused portion of the field. */


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
        ss << "  id_or_mac: [" << to_string(id_or_mac) << "]" << std::endl;
        ss << "  description_type: " << +description_type << std::endl;
        ss << "  description: \"" << to_string(description) << "\"" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << target_system;                 // offset: 0
        map << target_component;              // offset: 1
        map << id_or_mac;                     // offset: 2
        map << description_type;              // offset: 22
        map << description;                   // offset: 23
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> target_system;                 // offset: 0
        map >> target_component;              // offset: 1
        map >> id_or_mac;                     // offset: 2
        map >> description_type;              // offset: 22
        map >> description;                   // offset: 23
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
