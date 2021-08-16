#ifndef MUITEMCAMDATA_H
#define MUITEMCAMDATA_H

#include "workspace.h"
#include <QMetaType>


typedef struct {
    QString id;
    QString ver;
    QString status;
    QString name;
    QString speed;
    enum OPENVIO_TYPE type;
} MuItemCamData;

Q_DECLARE_METATYPE(MuItemCamData)

#endif
