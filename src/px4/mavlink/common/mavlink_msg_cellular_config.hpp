// MESSAGE CELLULAR_CONFIG support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief CELLULAR_CONFIG message
 *
 * Configure cellular modems. This message is re-emitted as an acknowledgement by the modem. The message may also be explicitly requested using MAV_CMD_REQUEST_MESSAGE.
 */
struct CELLULAR_CONFIG : mavlink::Message {
    static constexpr msgid_t MSG_ID = 336;
    static constexpr size_t LENGTH = 84;
    static constexpr size_t MIN_LENGTH = 84;
    static constexpr uint8_t CRC_EXTRA = 245;
    static constexpr auto NAME = "CELLULAR_CONFIG";


    uint8_t enable_lte; /*<  Enable/disable LTE. 0: setting unchanged, 1: disabled, 2: enabled. Current setting when sent back as a response. */
    uint8_t enable_pin; /*<  Enable/disable PIN on the SIM card. 0: setting unchanged, 1: disabled, 2: enabled. Current setting when sent back as a response. */
    std::array<char, 16> pin; /*<  PIN sent to the SIM card. Blank when PIN is disabled. Empty when message is sent back as a response. */
    std::array<char, 16> new_pin; /*<  New PIN when changing the PIN. Blank to leave it unchanged. Empty when message is sent back as a response. */
    std::array<char, 32> apn; /*<  Name of the cellular APN. Blank to leave it unchanged. Current APN when sent back as a response. */
    std::array<char, 16> puk; /*<  Required PUK code in case the user failed to authenticate 3 times with the PIN. Empty when message is sent back as a response. */
    uint8_t roaming; /*<  Enable/disable roaming. 0: setting unchanged, 1: disabled, 2: enabled. Current setting when sent back as a response. */
    uint8_t response; /*<  Message acceptance response (sent back to GS). */


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
        ss << "  enable_lte: " << +enable_lte << std::endl;
        ss << "  enable_pin: " << +enable_pin << std::endl;
        ss << "  pin: \"" << to_string(pin) << "\"" << std::endl;
        ss << "  new_pin: \"" << to_string(new_pin) << "\"" << std::endl;
        ss << "  apn: \"" << to_string(apn) << "\"" << std::endl;
        ss << "  puk: \"" << to_string(puk) << "\"" << std::endl;
        ss << "  roaming: " << +roaming << std::endl;
        ss << "  response: " << +response << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << enable_lte;                    // offset: 0
        map << enable_pin;                    // offset: 1
        map << pin;                           // offset: 2
        map << new_pin;                       // offset: 18
        map << apn;                           // offset: 34
        map << puk;                           // offset: 66
        map << roaming;                       // offset: 82
        map << response;                      // offset: 83
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> enable_lte;                    // offset: 0
        map >> enable_pin;                    // offset: 1
        map >> pin;                           // offset: 2
        map >> new_pin;                       // offset: 18
        map >> apn;                           // offset: 34
        map >> puk;                           // offset: 66
        map >> roaming;                       // offset: 82
        map >> response;                      // offset: 83
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
