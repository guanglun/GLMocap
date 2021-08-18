#ifndef __GL_PROTOCOL_CONFIG_H__
#define __GL_PROTOCOL_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <memory.h>

#define LOCAL_ID     0x01
#define TARGET_ID    0x02

#define FRAME_DATA_MAX_SIZE   (2048-11)

#ifndef uint8_t
    typedef unsigned char   uint8_t;
    typedef unsigned short  uint16_t;
    typedef unsigned int    uint32_t;
#endif

#ifdef __cplusplus
}
#endif

#endif
