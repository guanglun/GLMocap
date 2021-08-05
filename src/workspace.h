#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QDebug>
#include "setting.h"
#include "log.h"


#define QDBG(tag, args...) do{\
    char temp[4096] = tag;\
    sprintf(temp + 7, ##args);\
    qDebug()<<temp;\
}while(0)

#define ERR(args...)      QDBG("ERROR   ", ##args)
#define WARNING(args...)  QDBG("WARNING ", ##args)
#define DBG(args...)      QDBG("DEBUG   ", ##args)

extern Setting *setting;
extern Log *mlog;

typedef enum {
    PIXFORMAT_INVALID = 0,
    PIXFORMAT_BINARY,    // 1BPP/BINARY
    PIXFORMAT_GRAYSCALE, // 1BPP/GRAYSCALE
    PIXFORMAT_RGB565,    // 2BPP/RGB565
    PIXFORMAT_YUV422,    // 2BPP/YUV422
    PIXFORMAT_BAYER,     // 1BPP/RAW
    PIXFORMAT_JPEG,      // JPEG/COMPRESSED
} pixformat_t;

enum{
USB_MSG_OPEN_SUCCESS,
USB_MSG_CLOSE_SUCCESS,
USB_MSG_OPEN_FAIL
};

enum SENSOR_STATUS{
    SENSOR_STATUS_STOP,
    SENSOR_STATUS_WAIT_HEAD,
    SENSOR_STATUS_RUNNING
};

enum OPENVIO_TYPE {
    TYPE_NULL,
    TYPE_BOOTLOADER,
    TYPE_OPENVIO
    
};

#endif // WORKSPACE_H
