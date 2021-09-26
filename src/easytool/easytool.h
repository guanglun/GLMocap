#ifndef EASYTOOL_H
#define EASYTOOL_H

#include "workspace.h"
#include <QObject>
#include <QDir>


namespace EasyTool
{
    QByteArray HexStringToByteArray(QString HexString);
    QString ByteArrayToHexString(QByteArray data);
    bool isDirExist(QString fullPath);
    bool isFileExist(QString fullPath);
};

#endif // EASYTOOL_H
