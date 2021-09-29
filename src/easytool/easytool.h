#ifndef EASYTOOL_H
#define EASYTOOL_H

#include "workspace.h"
#include <QObject>
#include <QDir>

#include <Eigen/Dense>

namespace EasyTool
{
    QByteArray HexStringToByteArray(QString HexString);
    QString ByteArrayToHexString(QByteArray data);
    bool isDirExist(QString fullPath);
    bool isFileExist(QString fullPath);
    Eigen::Matrix<double, 4, 4> getRT44d(Eigen::Matrix<double, 3, 3> &R,Eigen::Matrix<double, 3, 1> &T);
    Eigen::Matrix<double, 3, 4> getRT34d(Eigen::Matrix<double, 3, 3> &R,Eigen::Matrix<double, 3, 1> &T);
    void RT44d(Eigen::Matrix<double, 4, 4> &RT,Eigen::Matrix<double, 3, 3> &R,Eigen::Matrix<double, 3, 1> &T);
    QString MatToString(const Eigen::MatrixXd& mat);
};

#endif // EASYTOOL_H
