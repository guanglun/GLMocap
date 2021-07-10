#ifndef MUITEMCAMDATA_H
#define MUITEMCAMDATA_H

#include <QMetaType>


typedef struct {
    QString id;
    QString status;
} MuItemCamData;

Q_DECLARE_METATYPE(MuItemCamData)

#endif
