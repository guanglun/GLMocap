// MESSAGE OPEN_DRONE_ID_BASIC_ID support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief OPEN_DRONE_ID_BASIC_ID message
 *
 * Data for filling the OpenDroneID Basic ID message. This and the below messages are primarily meant for feeding data to/from an OpenDroneID implementation. E.g. https://github.com/opendroneid/opendroneid-core-c. These messages are compatible with the ASTM Remote ID standard at https://www.astm.org/Standards/F3411.htm and the ASD-STAN Direct Remote ID standard. The usage of these messages is documented at https://mavlink.io/en/services/opendroneid.html.
 */
struct OPEN_DRONE_ID_BASIC_ID : mavlink::Message {
    static constexpr msgid_t MSG_ID = 12900;
    static constexpr size_t LENGTH = 44;
    static constexpr size_t MIN_LENGTH = 44;
    static constexpr uint8_t CRC_EXTRA = 114;
    static constexpr auto NAME = "OPEN_DRONE_ID_BASIC_ID";


    uint8_t target_system; /*<  System ID (0 for broadcast). */
    uint8_t target_component; /*<  Component ID (0 for broadcast). */
    std::array<uint8_t, 20> id_or_mac; /*<  Only used for drone ID data received from other UAs. See detailed description at https://mavlink.io/en/services/opendroneid.html.  */
    uint8_t id_type; /*<  Indicates the format for the uas_id field of this message. */
    uint8_t ua_type; /*<  Indicates the type of UA (Unmanned Aircraft). */
    std::array<uint8_t, 20> uas_id; /*<  UAS (Unmanned Aircraft System) ID following the format specified by id_type. Shall be filled with nulls in the unused portion of the field. */


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
        ss << "  id_type: " << +id_type << std::endl;
        ss << "  ua_type: " << +ua_type << std::endl;
        ss << "  uas_id: [" << to_string(uas_id) << "]" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << target_system;                 // offset: 0
        map << target_component;              // offset: 1
        map << id_or_mac;                     // offset: 2
        map << id_type;                       // offset: 22
        map << ua_type;                       // offset: 23
        map << uas_id;                        // offset: 24
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> target_system;                 // offset: 0
        map >> target_component;              // offset: 1
        map >> id_or_mac;                     // offset: 2
        map >> id_type;                       // offset: 22
        map >> ua_type;                       // offset: 23
        map >> uas_id;                        // offset: 24
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
