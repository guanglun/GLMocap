// MESSAGE TRAJECTORY_REPRESENTATION_BEZIER support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief TRAJECTORY_REPRESENTATION_BEZIER message
 *
 * Describe a trajectory using an array of up-to 5 bezier control points in the local frame (MAV_FRAME_LOCAL_NED).
 */
struct TRAJECTORY_REPRESENTATION_BEZIER : mavlink::Message {
    static constexpr msgid_t MSG_ID = 333;
    static constexpr size_t LENGTH = 109;
    static constexpr size_t MIN_LENGTH = 109;
    static constexpr uint8_t CRC_EXTRA = 231;
    static constexpr auto NAME = "TRAJECTORY_REPRESENTATION_BEZIER";


    uint64_t time_usec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number. */
    uint8_t valid_points; /*<  Number of valid control points (up-to 5 points are possible) */
    std::array<float, 5> pos_x; /*< [m] X-coordinate of bezier control points. Set to NaN if not being used */
    std::array<float, 5> pos_y; /*< [m] Y-coordinate of bezier control points. Set to NaN if not being used */
    std::array<float, 5> pos_z; /*< [m] Z-coordinate of bezier control points. Set to NaN if not being used */
    std::array<float, 5> delta; /*< [s] Bezier time horizon. Set to NaN if velocity/acceleration should not be incorporated */
    std::array<float, 5> pos_yaw; /*< [rad] Yaw. Set to NaN for unchanged */


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
        ss << "  time_usec: " << time_usec << std::endl;
        ss << "  valid_points: " << +valid_points << std::endl;
        ss << "  pos_x: [" << to_string(pos_x) << "]" << std::endl;
        ss << "  pos_y: [" << to_string(pos_y) << "]" << std::endl;
        ss << "  pos_z: [" << to_string(pos_z) << "]" << std::endl;
        ss << "  delta: [" << to_string(delta) << "]" << std::endl;
        ss << "  pos_yaw: [" << to_string(pos_yaw) << "]" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << time_usec;                     // offset: 0
        map << pos_x;                         // offset: 8
        map << pos_y;                         // offset: 28
        map << pos_z;                         // offset: 48
        map << delta;                         // offset: 68
        map << pos_yaw;                       // offset: 88
        map << valid_points;                  // offset: 108
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> time_usec;                     // offset: 0
        map >> pos_x;                         // offset: 8
        map >> pos_y;                         // offset: 28
        map >> pos_z;                         // offset: 48
        map >> delta;                         // offset: 68
        map >> pos_yaw;                       // offset: 88
        map >> valid_points;                  // offset: 108
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
