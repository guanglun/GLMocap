// MESSAGE CAMERA_FOV_STATUS support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief CAMERA_FOV_STATUS message
 *
 * Information about the field of view of a camera. Can be requested with a MAV_CMD_REQUEST_MESSAGE command.
 */
struct CAMERA_FOV_STATUS : mavlink::Message {
    static constexpr msgid_t MSG_ID = 271;
    static constexpr size_t LENGTH = 52;
    static constexpr size_t MIN_LENGTH = 52;
    static constexpr uint8_t CRC_EXTRA = 22;
    static constexpr auto NAME = "CAMERA_FOV_STATUS";


    uint32_t time_boot_ms; /*< [ms] Timestamp (time since system boot). */
    int32_t lat_camera; /*< [degE7] Latitude of camera (INT32_MAX if unknown). */
    int32_t lon_camera; /*< [degE7] Longitude of camera (INT32_MAX if unknown). */
    int32_t alt_camera; /*< [mm] Altitude (MSL) of camera (INT32_MAX if unknown). */
    int32_t lat_image; /*< [degE7] Latitude of center of image (INT32_MAX if unknown, INT32_MIN if at infinity, not intersecting with horizon). */
    int32_t lon_image; /*< [degE7] Longitude of center of image (INT32_MAX if unknown, INT32_MIN if at infinity, not intersecting with horizon). */
    int32_t alt_image; /*< [mm] Altitude (MSL) of center of image (INT32_MAX if unknown, INT32_MIN if at infinity, not intersecting with horizon). */
    std::array<float, 4> q; /*<  Quaternion of camera orientation (w, x, y, z order, zero-rotation is 1, 0, 0, 0) */
    float hfov; /*< [deg] Horizontal field of view (NaN if unknown). */
    float vfov; /*< [deg] Vertical field of view (NaN if unknown). */


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
        ss << "  lat_camera: " << lat_camera << std::endl;
        ss << "  lon_camera: " << lon_camera << std::endl;
        ss << "  alt_camera: " << alt_camera << std::endl;
        ss << "  lat_image: " << lat_image << std::endl;
        ss << "  lon_image: " << lon_image << std::endl;
        ss << "  alt_image: " << alt_image << std::endl;
        ss << "  q: [" << to_string(q) << "]" << std::endl;
        ss << "  hfov: " << hfov << std::endl;
        ss << "  vfov: " << vfov << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << time_boot_ms;                  // offset: 0
        map << lat_camera;                    // offset: 4
        map << lon_camera;                    // offset: 8
        map << alt_camera;                    // offset: 12
        map << lat_image;                     // offset: 16
        map << lon_image;                     // offset: 20
        map << alt_image;                     // offset: 24
        map << q;                             // offset: 28
        map << hfov;                          // offset: 44
        map << vfov;                          // offset: 48
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> time_boot_ms;                  // offset: 0
        map >> lat_camera;                    // offset: 4
        map >> lon_camera;                    // offset: 8
        map >> alt_camera;                    // offset: 12
        map >> lat_image;                     // offset: 16
        map >> lon_image;                     // offset: 20
        map >> alt_image;                     // offset: 24
        map >> q;                             // offset: 28
        map >> hfov;                          // offset: 44
        map >> vfov;                          // offset: 48
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
