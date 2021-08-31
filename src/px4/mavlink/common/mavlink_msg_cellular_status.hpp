// MESSAGE CELLULAR_STATUS support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief CELLULAR_STATUS message
 *
 * Report current used cellular network status
 */
struct CELLULAR_STATUS : mavlink::Message {
    static constexpr msgid_t MSG_ID = 334;
    static constexpr size_t LENGTH = 10;
    static constexpr size_t MIN_LENGTH = 10;
    static constexpr uint8_t CRC_EXTRA = 72;
    static constexpr auto NAME = "CELLULAR_STATUS";


    uint8_t status; /*<  Cellular modem status */
    uint8_t failure_reason; /*<  Failure reason when status in in CELLUAR_STATUS_FAILED */
    uint8_t type; /*<  Cellular network radio type: gsm, cdma, lte... */
    uint8_t quality; /*<  Signal quality in percent. If unknown, set to UINT8_MAX */
    uint16_t mcc; /*<  Mobile country code. If unknown, set to UINT16_MAX */
    uint16_t mnc; /*<  Mobile network code. If unknown, set to UINT16_MAX */
    uint16_t lac; /*<  Location area code. If unknown, set to 0 */


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
        ss << "  status: " << +status << std::endl;
        ss << "  failure_reason: " << +failure_reason << std::endl;
        ss << "  type: " << +type << std::endl;
        ss << "  quality: " << +quality << std::endl;
        ss << "  mcc: " << mcc << std::endl;
        ss << "  mnc: " << mnc << std::endl;
        ss << "  lac: " << lac << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << mcc;                           // offset: 0
        map << mnc;                           // offset: 2
        map << lac;                           // offset: 4
        map << status;                        // offset: 6
        map << failure_reason;                // offset: 7
        map << type;                          // offset: 8
        map << quality;                       // offset: 9
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> mcc;                           // offset: 0
        map >> mnc;                           // offset: 2
        map >> lac;                           // offset: 4
        map >> status;                        // offset: 6
        map >> failure_reason;                // offset: 7
        map >> type;                          // offset: 8
        map >> quality;                       // offset: 9
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
