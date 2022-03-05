#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QDebug>
#include <QMap>
#include "setting.h"
#include "log.h"
#include "EasyTool.h"



//#define __ENABLE_3D__



#ifdef MSVC
    #define QLOG(fmt,...) do{\
        char temp[4096];\
        sprintf_s(temp,fmt, ##__VA_ARGS__);\
        mlog->show(temp);\
    }while(0)

    #define ERR(fmt, ...)      
    #define WARNING(fmt, ...)  
    #define DBG(fmt,...)      QLOG(fmt, __VA_ARGS__)
#endif

//#ifdef __MINGW32__
#ifdef __GNUC__

#define QDBG(tag, args...) do{\
    char temp[4096] = tag;\
    sprintf(temp + 7, ##args);\
    mlog->show(temp);\
}while(0)

#define QLOG(args...) do{\
    char temp[4096];\
    sprintf(temp, ##args);\
    mlog->show(temp);\
}while(0)

#define ERR(args...)      QDBG("ERROR   ", ##args)
#define WARNING(args...)  QDBG("WARNING ", ##args)
#define DBG(args...)      QLOG(args)

#endif

// #define errprintf(fmt, ...){fprintf(stderr, __FUNCTION__ fmt, __VAR_ARGS__); }

//#define DBG(args...)      QDBG("DEBUG   ", ##args)

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
    SENSOR_STATUS_START,
    SENSOR_STATUS_RUNNING
};

enum OPENVIO_TYPE {
    TYPE_NULL,
    TYPE_BOOTLOADER,
    TYPE_OPENVIO,
    TYPE_VIRTUAL
};

#endif // WORKSPACE_H
