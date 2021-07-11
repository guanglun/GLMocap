#include "setting.h"

Setting::Setting()
{
    QApplication::setOrganizationName("openvio");
    QApplication::setOrganizationDomain("openvio.com");
    QApplication::setApplicationName("openvio");

    QString setFile= QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/.OPENVIOSettings.ini";
    qDebug() << "配置文件路径：" << setFile;
    set = new QSettings(setFile,QSettings::IniFormat);    
    
    getImagePath();
}

void Setting::getIMUOffset(T_int16_xyz *acc,T_int16_xyz *gyro)
{
    QString acc_offset_x  = set->value(ACC_OFFSET_X.SET_NAME).toString();
    QString acc_offset_y  = set->value(ACC_OFFSET_Y.SET_NAME).toString();
    QString acc_offset_z  = set->value(ACC_OFFSET_Z.SET_NAME).toString();
    
    QString gyr_offset_x  = set->value(GYR_OFFSET_X.SET_NAME).toString();
    QString gyr_offset_y  = set->value(GYR_OFFSET_Y.SET_NAME).toString();
    QString gyr_offset_z  = set->value(GYR_OFFSET_Z.SET_NAME).toString();
    
    if(acc_offset_x.isEmpty())
    {
        set->setValue(ACC_OFFSET_X.SET_NAME,ACC_OFFSET_X.DEFAULT_VALUE);
        acc_offset_x = ACC_OFFSET_X.DEFAULT_VALUE;
    }
    acc->X = acc_offset_x.toInt();
 
    
    if(acc_offset_y.isEmpty())
    {
        set->setValue(ACC_OFFSET_Y.SET_NAME,ACC_OFFSET_Y.DEFAULT_VALUE);
        acc_offset_y = ACC_OFFSET_Y.DEFAULT_VALUE;
    }
    acc->Y = acc_offset_y.toInt();
    
    
    if(acc_offset_z.isEmpty())
    {
        set->setValue(ACC_OFFSET_Z.SET_NAME,ACC_OFFSET_Z.DEFAULT_VALUE);
        acc_offset_z = ACC_OFFSET_Z.DEFAULT_VALUE;
    }
    acc->Z = acc_offset_z.toInt();
    
    if(gyr_offset_x.isEmpty())
    {
        set->setValue(GYR_OFFSET_X.SET_NAME,GYR_OFFSET_X.DEFAULT_VALUE);
        gyr_offset_x = GYR_OFFSET_X.DEFAULT_VALUE;
    }
    gyro->X = gyr_offset_x.toInt();
    
    if(gyr_offset_y.isEmpty())
    {
        set->setValue(GYR_OFFSET_Y.SET_NAME,GYR_OFFSET_Y.DEFAULT_VALUE);
        gyr_offset_y = GYR_OFFSET_Y.DEFAULT_VALUE;
    }
    gyro->Y = gyr_offset_y.toInt();
    
    if(gyr_offset_z.isEmpty())
    {
        set->setValue(GYR_OFFSET_Z.SET_NAME,GYR_OFFSET_Z.DEFAULT_VALUE);
        gyr_offset_z = GYR_OFFSET_Z.DEFAULT_VALUE;
    }
    gyro->Z = gyr_offset_z.toInt();
    
    qDebug() << "acc offset x:" << acc->X;
    qDebug() << "acc offset y:" << acc->Y;
    qDebug() << "acc offset z:" << acc->Z;
    
    qDebug() << "gyr offset x:" << gyro->X;
    qDebug() << "gyr offset y:" << gyro->Y;
    qDebug() << "gyr offset z:" << gyro->Z;

    
}

void Setting::setAccOffset(T_int16_xyz *acc)
{
    set->setValue(ACC_OFFSET_X.SET_NAME,QString::number(acc->X));
    set->setValue(ACC_OFFSET_Y.SET_NAME,QString::number(acc->Y));
    set->setValue(ACC_OFFSET_Z.SET_NAME,QString::number(acc->Z));
}

void Setting::setGyroOffset(T_int16_xyz *gyro)
{
    set->setValue(GYR_OFFSET_X.SET_NAME,QString::number(gyro->X));
    set->setValue(GYR_OFFSET_Y.SET_NAME,QString::number(gyro->Y));
    set->setValue(GYR_OFFSET_Z.SET_NAME,QString::number(gyro->Z));
}

void Setting::setNameById(QString id,QString name)
{
    set->beginGroup("openvio");
    set->setValue(id,name);
    set->endGroup(); 
}

QString Setting::getNameById(QString id)
{
    QString name;
    set->beginGroup("openvio");
    name = set->value(id).toString();
    set->endGroup(); 
    return name;
}

void Setting::setImagePath(QString path)
{
    imagePath = path;
    set->setValue("IMAGE_PATH",path);
}

QString Setting::getImagePath()
{
    imagePath = set->value("IMAGE_PATH").toString();
    return imagePath;
}

