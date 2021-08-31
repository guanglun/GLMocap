// MESSAGE OPEN_DRONE_ID_OPERATOR_ID support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief OPEN_DRONE_ID_OPERATOR_ID message
 *
 * Data for filling the OpenDroneID Operator ID message, which contains the CAA (Civil Aviation Authority) issued operator ID.
 */
struct OPEN_DRONE_ID_OPERATOR_ID : mavlink::Message {
    static constexpr msgid_t MSG_ID = 12905;
    static constexpr size_t LENGTH = 43;
    static constexpr size_t MIN_LENGTH = 43;
    static constexpr uint8_t CRC_EXTRA = 49;
    static constexpr auto NAME = "OPEN_DRONE_ID_OPERATOR_ID";


    uint8_t target_system; /*<  System ID (0 for broadcast). */
    uint8_t target_component; /*<  Component ID (0 for broadcast). */
    std::array<uint8_t, 20> id_or_mac; /*<  Only used for drone ID data received from other UAs. See detailed description at https://mavlink.io/en/services/opendroneid.html.  */
    uint8_t operator_id_type; /*<  Indicates the type of the operator_id field. */
    std::array<char, 20> operator_id; /*<  Text description or numeric value expressed as ASCII characters. Shall be filled with nulls in the unused portion of the field. */


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
        ss << "  operator_id_type: " << +operator_id_type << std::endl;
        ss << "  operator_id: \"" << to_string(operator_id) << "\"" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << target_system;                 // offset: 0
        map << target_component;              // offset: 1
        map << id_or_mac;                     // offset: 2
        map << operator_id_type;              // offset: 22
        map << operator_id;                   // offset: 23
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> target_system;                 // offset: 0
        map >> target_component;              // offset: 1
        map >> id_or_mac;                     // offset: 2
        map >> operator_id_type;              // offset: 22
        map >> operator_id;                   // offset: 23
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
