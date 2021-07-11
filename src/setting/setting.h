#ifndef SETTING_H
#define SETTING_H

#include <QMainWindow>
#include <QDebug>
#include <QList>
#include <QSettings>
#include <QFile>
#include <QStringList>
#include <QDesktopServices>
#include <QApplication>
#include <QObject>

#include "imu.h"


class Setting : public QObject
{
    Q_OBJECT
private:
    struct SET_STRUCT{
        QString SET_NAME;
        QString DEFAULT_VALUE;
    };

    struct SET_STRUCT ACC_OFFSET_X = {"ACC_OFFSET_X","0"};
    struct SET_STRUCT ACC_OFFSET_Y = {"ACC_OFFSET_Y","0"};
    struct SET_STRUCT ACC_OFFSET_Z = {"ACC_OFFSET_Z","0"};
    struct SET_STRUCT GYR_OFFSET_X = {"GYR_OFFSET_X","0"};
    struct SET_STRUCT GYR_OFFSET_Y = {"GYR_OFFSET_Y","0"};
    struct SET_STRUCT GYR_OFFSET_Z = {"GYR_OFFSET_Z","0"};
    
    
    QSettings *set;
    QString imagePath;
public:
    Setting();
    void getIMUOffset(T_int16_xyz *acc,T_int16_xyz *gyro);
    void setAccOffset(T_int16_xyz *acc);
    void setGyroOffset(T_int16_xyz *gyro);
    void setNameById(QString id,QString name);
    QString getNameById(QString id);
    void setImagePath(QString path);
    QString getImagePath();
};

#endif // SETTING_H
