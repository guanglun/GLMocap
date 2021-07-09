#ifndef EASYTOOL_H
#define EASYTOOL_H

#include "workspace.h"
#include <QObject>

class EasyTool : public QObject
{
    Q_OBJECT
public:
    EasyTool();
    QByteArray HexStringToByteArray(QString HexString);
    static QString ByteArrayToHexString(QByteArray data);
};

#endif // EASYTOOL_H
