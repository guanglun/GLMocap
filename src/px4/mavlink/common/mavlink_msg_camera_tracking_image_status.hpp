// MESSAGE CAMERA_TRACKING_IMAGE_STATUS support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief CAMERA_TRACKING_IMAGE_STATUS message
 *
 * Camera tracking status, sent while in active tracking. Use MAV_CMD_SET_MESSAGE_INTERVAL to define message interval.
 */
struct CAMERA_TRACKING_IMAGE_STATUS : mavlink::Message {
    static constexpr msgid_t MSG_ID = 275;
    static constexpr size_t LENGTH = 31;
    static constexpr size_t MIN_LENGTH = 31;
    static constexpr uint8_t CRC_EXTRA = 126;
    static constexpr auto NAME = "CAMERA_TRACKING_IMAGE_STATUS";


    uint8_t tracking_status; /*<  Current tracking status */
    uint8_t tracking_mode; /*<  Current tracking mode */
    uint8_t target_data; /*<  Defines location of target data */
    float point_x; /*<  Current tracked point x value if CAMERA_TRACKING_MODE_POINT (normalized 0..1, 0 is left, 1 is right), NAN if unknown */
    float point_y; /*<  Current tracked point y value if CAMERA_TRACKING_MODE_POINT (normalized 0..1, 0 is top, 1 is bottom), NAN if unknown */
    float radius; /*<  Current tracked radius if CAMERA_TRACKING_MODE_POINT (normalized 0..1, 0 is image left, 1 is image right), NAN if unknown */
    float rec_top_x; /*<  Current tracked rectangle top x value if CAMERA_TRACKING_MODE_RECTANGLE (normalized 0..1, 0 is left, 1 is right), NAN if unknown */
    float rec_top_y; /*<  Current tracked rectangle top y value if CAMERA_TRACKING_MODE_RECTANGLE (normalized 0..1, 0 is top, 1 is bottom), NAN if unknown */
    float rec_bottom_x; /*<  Current tracked rectangle bottom x value if CAMERA_TRACKING_MODE_RECTANGLE (normalized 0..1, 0 is left, 1 is right), NAN if unknown */
    float rec_bottom_y; /*<  Current tracked rectangle bottom y value if CAMERA_TRACKING_MODE_RECTANGLE (normalized 0..1, 0 is top, 1 is bottom), NAN if unknown */


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
        ss << "  tracking_status: " << +tracking_status << std::endl;
        ss << "  tracking_mode: " << +tracking_mode << std::endl;
        ss << "  target_data: " << +target_data << std::endl;
        ss << "  point_x: " << point_x << std::endl;
        ss << "  point_y: " << point_y << std::endl;
        ss << "  radius: " << radius << std::endl;
        ss << "  rec_top_x: " << rec_top_x << std::endl;
        ss << "  rec_top_y: " << rec_top_y << std::endl;
        ss << "  rec_bottom_x: " << rec_bottom_x << std::endl;
        ss << "  rec_bottom_y: " << rec_bottom_y << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << point_x;                       // offset: 0
        map << point_y;                       // offset: 4
        map << radius;                        // offset: 8
        map << rec_top_x;                     // offset: 12
        map << rec_top_y;                     // offset: 16
        map << rec_bottom_x;                  // offset: 20
        map << rec_bottom_y;                  // offset: 24
        map << tracking_status;               // offset: 28
        map << tracking_mode;                 // offset: 29
        map << target_data;                   // offset: 30
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> point_x;                       // offset: 0
        map >> point_y;                       // offset: 4
        map >> radius;                        // offset: 8
        map >> rec_top_x;                     // offset: 12
        map >> rec_top_y;                     // offset: 16
        map >> rec_bottom_x;                  // offset: 20
        map >> rec_bottom_y;                  // offset: 24
        map >> tracking_status;               // offset: 28
        map >> tracking_mode;                 // offset: 29
        map >> target_data;                   // offset: 30
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
