// MESSAGE OPEN_DRONE_ID_AUTHENTICATION support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief OPEN_DRONE_ID_AUTHENTICATION message
 *
 * Data for filling the OpenDroneID Authentication message. The Authentication Message defines a field that can provide a means of authenticity for the identity of the UAS (Unmanned Aircraft System). The Authentication message can have two different formats. Five data pages are supported. For data page 0, the fields PageCount, Length and TimeStamp are present and AuthData is only 17 bytes. For data page 1 through 4, PageCount, Length and TimeStamp are not present and the size of AuthData is 23 bytes.
 */
struct OPEN_DRONE_ID_AUTHENTICATION : mavlink::Message {
    static constexpr msgid_t MSG_ID = 12902;
    static constexpr size_t LENGTH = 53;
    static constexpr size_t MIN_LENGTH = 53;
    static constexpr uint8_t CRC_EXTRA = 49;
    static constexpr auto NAME = "OPEN_DRONE_ID_AUTHENTICATION";


    uint8_t target_system; /*<  System ID (0 for broadcast). */
    uint8_t target_component; /*<  Component ID (0 for broadcast). */
    std::array<uint8_t, 20> id_or_mac; /*<  Only used for drone ID data received from other UAs. See detailed description at https://mavlink.io/en/services/opendroneid.html.  */
    uint8_t authentication_type; /*<  Indicates the type of authentication. */
    uint8_t data_page; /*<  Allowed range is 0 - 4. */
    uint8_t page_count; /*<  This field is only present for page 0. Allowed range is 0 - 5. */
    uint8_t length; /*< [bytes] This field is only present for page 0. Total bytes of authentication_data from all data pages. Allowed range is 0 - 109 (17 + 23*4). */
    uint32_t timestamp; /*< [s] This field is only present for page 0. 32 bit Unix Timestamp in seconds since 00:00:00 01/01/2019. */
    std::array<uint8_t, 23> authentication_data; /*<  Opaque authentication data. For page 0, the size is only 17 bytes. For other pages, the size is 23 bytes. Shall be filled with nulls in the unused portion of the field. */


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
        ss << "  authentication_type: " << +authentication_type << std::endl;
        ss << "  data_page: " << +data_page << std::endl;
        ss << "  page_count: " << +page_count << std::endl;
        ss << "  length: " << +length << std::endl;
        ss << "  timestamp: " << timestamp << std::endl;
        ss << "  authentication_data: [" << to_string(authentication_data) << "]" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << timestamp;                     // offset: 0
        map << target_system;                 // offset: 4
        map << target_component;              // offset: 5
        map << id_or_mac;                     // offset: 6
        map << authentication_type;           // offset: 26
        map << data_page;                     // offset: 27
        map << page_count;                    // offset: 28
        map << length;                        // offset: 29
        map << authentication_data;           // offset: 30
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> timestamp;                     // offset: 0
        map >> target_system;                 // offset: 4
        map >> target_component;              // offset: 5
        map >> id_or_mac;                     // offset: 6
        map >> authentication_type;           // offset: 26
        map >> data_page;                     // offset: 27
        map >> page_count;                    // offset: 28
        map >> length;                        // offset: 29
        map >> authentication_data;           // offset: 30
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
