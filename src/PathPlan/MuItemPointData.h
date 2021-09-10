#ifndef MUITEMPOINTDATA_H
#define MUITEMPOINTDATA_H

#include "workspace.h"
#include <QMetaType>

typedef struct {
    QString stateString;
    QString typeString;
    float x;
    float y;
    float z;
    int waitTime;
} MuItemPointData;

Q_DECLARE_METATYPE(MuItemPointData)

#endif
