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

#endif // WORKSPACE_H
