#ifndef MUITEMCAMDATA_H
#define MUITEMCAMDATA_H

#include "workspace.h"
#include <QMetaType>


typedef struct {
    QString id;
    QString status;
    QString name;
    enum OPENVIO_TYPE type;
} MuItemCamData;

Q_DECLARE_METATYPE(MuItemCamData)

#endif
