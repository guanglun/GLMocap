// MESSAGE OPEN_DRONE_ID_LOCATION support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief OPEN_DRONE_ID_LOCATION message
 *
 * Data for filling the OpenDroneID Location message. The float data types are 32-bit IEEE 754. The Location message provides the location, altitude, direction and speed of the aircraft.
 */
struct OPEN_DRONE_ID_LOCATION : mavlink::Message {
    static constexpr msgid_t MSG_ID = 12901;
    static constexpr size_t LENGTH = 59;
    static constexpr size_t MIN_LENGTH = 59;
    static constexpr uint8_t CRC_EXTRA = 254;
    static constexpr auto NAME = "OPEN_DRONE_ID_LOCATION";


    uint8_t target_system; /*<  System ID (0 for broadcast). */
    uint8_t target_component; /*<  Component ID (0 for broadcast). */
    std::array<uint8_t, 20> id_or_mac; /*<  Only used for drone ID data received from other UAs. See detailed description at https://mavlink.io/en/services/opendroneid.html.  */
    uint8_t status; /*<  Indicates whether the unmanned aircraft is on the ground or in the air. */
    uint16_t direction; /*< [cdeg] Direction over ground (not heading, but direction of movement) measured clockwise from true North: 0 - 35999 centi-degrees. If unknown: 36100 centi-degrees. */
    uint16_t speed_horizontal; /*< [cm/s] Ground speed. Positive only. If unknown: 25500 cm/s. If speed is larger than 25425 cm/s, use 25425 cm/s. */
    int16_t speed_vertical; /*< [cm/s] The vertical speed. Up is positive. If unknown: 6300 cm/s. If speed is larger than 6200 cm/s, use 6200 cm/s. If lower than -6200 cm/s, use -6200 cm/s. */
    int32_t latitude; /*< [degE7] Current latitude of the unmanned aircraft. If unknown: 0 (both Lat/Lon). */
    int32_t longitude; /*< [degE7] Current longitude of the unmanned aircraft. If unknown: 0 (both Lat/Lon). */
    float altitude_barometric; /*< [m] The altitude calculated from the barometric pressue. Reference is against 29.92inHg or 1013.2mb. If unknown: -1000 m. */
    float altitude_geodetic; /*< [m] The geodetic altitude as defined by WGS84. If unknown: -1000 m. */
    uint8_t height_reference; /*<  Indicates the reference point for the height field. */
    float height; /*< [m] The current height of the unmanned aircraft above the take-off location or the ground as indicated by height_reference. If unknown: -1000 m. */
    uint8_t horizontal_accuracy; /*<  The accuracy of the horizontal position. */
    uint8_t vertical_accuracy; /*<  The accuracy of the vertical position. */
    uint8_t barometer_accuracy; /*<  The accuracy of the barometric altitude. */
    uint8_t speed_accuracy; /*<  The accuracy of the horizontal and vertical speed. */
    float timestamp; /*< [s] Seconds after the full hour with reference to UTC time. Typically the GPS outputs a time-of-week value in milliseconds. First convert that to UTC and then convert for this field using ((float) (time_week_ms % (60*60*1000))) / 1000. */
    uint8_t timestamp_accuracy; /*<  The accuracy of the timestamps. */


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
        ss << "  status: " << +status << std::endl;
        ss << "  direction: " << direction << std::endl;
        ss << "  speed_horizontal: " << speed_horizontal << std::endl;
        ss << "  speed_vertical: " << speed_vertical << std::endl;
        ss << "  latitude: " << latitude << std::endl;
        ss << "  longitude: " << longitude << std::endl;
        ss << "  altitude_barometric: " << altitude_barometric << std::endl;
        ss << "  altitude_geodetic: " << altitude_geodetic << std::endl;
        ss << "  height_reference: " << +height_reference << std::endl;
        ss << "  height: " << height << std::endl;
        ss << "  horizontal_accuracy: " << +horizontal_accuracy << std::endl;
        ss << "  vertical_accuracy: " << +vertical_accuracy << std::endl;
        ss << "  barometer_accuracy: " << +barometer_accuracy << std::endl;
        ss << "  speed_accuracy: " << +speed_accuracy << std::endl;
        ss << "  timestamp: " << timestamp << std::endl;
        ss << "  timestamp_accuracy: " << +timestamp_accuracy << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << latitude;                      // offset: 0
        map << longitude;                     // offset: 4
        map << altitude_barometric;           // offset: 8
        map << altitude_geodetic;             // offset: 12
        map << height;                        // offset: 16
        map << timestamp;                     // offset: 20
        map << direction;                     // offset: 24
        map << speed_horizontal;              // offset: 26
        map << speed_vertical;                // offset: 28
        map << target_system;                 // offset: 30
        map << target_component;              // offset: 31
        map << id_or_mac;                     // offset: 32
        map << status;                        // offset: 52
        map << height_reference;              // offset: 53
        map << horizontal_accuracy;           // offset: 54
        map << vertical_accuracy;             // offset: 55
        map << barometer_accuracy;            // offset: 56
        map << speed_accuracy;                // offset: 57
        map << timestamp_accuracy;            // offset: 58
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> latitude;                      // offset: 0
        map >> longitude;                     // offset: 4
        map >> altitude_barometric;           // offset: 8
        map >> altitude_geodetic;             // offset: 12
        map >> height;                        // offset: 16
        map >> timestamp;                     // offset: 20
        map >> direction;                     // offset: 24
        map >> speed_horizontal;              // offset: 26
        map >> speed_vertical;                // offset: 28
        map >> target_system;                 // offset: 30
        map >> target_component;              // offset: 31
        map >> id_or_mac;                     // offset: 32
        map >> status;                        // offset: 52
        map >> height_reference;              // offset: 53
        map >> horizontal_accuracy;           // offset: 54
        map >> vertical_accuracy;             // offset: 55
        map >> barometer_accuracy;            // offset: 56
        map >> speed_accuracy;                // offset: 57
        map >> timestamp_accuracy;            // offset: 58
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
