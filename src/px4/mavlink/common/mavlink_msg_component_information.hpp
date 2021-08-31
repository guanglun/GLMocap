// MESSAGE COMPONENT_INFORMATION support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief COMPONENT_INFORMATION message
 *
 * Information about a component. For camera components instead use CAMERA_INFORMATION, and for autopilots additionally use AUTOPILOT_VERSION. Components including GCSes should consider supporting requests of this message via MAV_CMD_REQUEST_MESSAGE.
 */
struct COMPONENT_INFORMATION : mavlink::Message {
    static constexpr msgid_t MSG_ID = 395;
    static constexpr size_t LENGTH = 212;
    static constexpr size_t MIN_LENGTH = 212;
    static constexpr uint8_t CRC_EXTRA = 0;
    static constexpr auto NAME = "COMPONENT_INFORMATION";


    uint32_t time_boot_ms; /*< [ms] Timestamp (time since system boot). */
    uint32_t general_metadata_file_crc; /*<  CRC32 of the TYPE_GENERAL file (can be used by a GCS for file caching). */
    std::array<char, 100> general_metadata_uri; /*<  Component definition URI for TYPE_GENERAL. This must be a MAVLink FTP URI and the file might be compressed with xz. */
    uint32_t peripherals_metadata_file_crc; /*<  CRC32 of the TYPE_PERIPHERALS file (can be used by a GCS for file caching). */
    std::array<char, 100> peripherals_metadata_uri; /*<  (Optional) Component definition URI for TYPE_PERIPHERALS. This must be a MAVLink FTP URI and the file might be compressed with xz. */


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
        ss << "  time_boot_ms: " << time_boot_ms << std::endl;
        ss << "  general_metadata_file_crc: " << general_metadata_file_crc << std::endl;
        ss << "  general_metadata_uri: \"" << to_string(general_metadata_uri) << "\"" << std::endl;
        ss << "  peripherals_metadata_file_crc: " << peripherals_metadata_file_crc << std::endl;
        ss << "  peripherals_metadata_uri: \"" << to_string(peripherals_metadata_uri) << "\"" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << time_boot_ms;                  // offset: 0
        map << general_metadata_file_crc;     // offset: 4
        map << peripherals_metadata_file_crc; // offset: 8
        map << general_metadata_uri;          // offset: 12
        map << peripherals_metadata_uri;      // offset: 112
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> time_boot_ms;                  // offset: 0
        map >> general_metadata_file_crc;     // offset: 4
        map >> peripherals_metadata_file_crc; // offset: 8
        map >> general_metadata_uri;          // offset: 12
        map >> peripherals_metadata_uri;      // offset: 112
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
