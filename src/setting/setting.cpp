#include "setting.h"
#include "MultipleViewTriangulation.h"
#include <iostream>

Setting::Setting()
{
    QApplication::setOrganizationName("openvio");
    QApplication::setOrganizationDomain("openvio.com");
    QApplication::setApplicationName("openvio");

    QString setFile= QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/.settings.ini";
    qDebug() << "configure file path:" << setFile;
    set = new QSettings(setFile,QSettings::IniFormat);    

    getImagePath();
}

void Setting::getIMUOffset(T_int16_xyz *acc,T_int16_xyz *gyro)
{
    set->beginGroup("openvio");

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

    set->endGroup(); 
}

void Setting::setAccOffset(T_int16_xyz *acc)
{
    set->beginGroup("openvio");
    set->setValue(ACC_OFFSET_X.SET_NAME,QString::number(acc->X));
    set->setValue(ACC_OFFSET_Y.SET_NAME,QString::number(acc->Y));
    set->setValue(ACC_OFFSET_Z.SET_NAME,QString::number(acc->Z));
    set->endGroup(); 
}

void Setting::setGyroOffset(T_int16_xyz *gyro)
{
    set->beginGroup("openvio");
    set->setValue(GYR_OFFSET_X.SET_NAME,QString::number(gyro->X));
    set->setValue(GYR_OFFSET_Y.SET_NAME,QString::number(gyro->Y));
    set->setValue(GYR_OFFSET_Z.SET_NAME,QString::number(gyro->Z));
    set->endGroup(); 
}

void Setting::setNumberById(QString id,int number)
{
    set->beginGroup("openvio");
    set->setValue(id,number);
    set->endGroup(); 
}

int Setting::getNumberById(QString id)
{
    int number = -1;
    set->beginGroup("openvio");
    if(set->contains(id))
        number = set->value(id).toInt();
    set->endGroup(); 
    return number;
}

void Setting::setImagePath(QString path)
{
    set->beginGroup("openvio");
    imagePath = path;
    set->setValue("IMAGE_PATH",path);
    set->endGroup(); 
}

QString Setting::getImagePath()
{
    set->beginGroup("openvio");
    imagePath = set->value("IMAGE_PATH").toString();
    set->endGroup(); 
    return imagePath;
}

void Setting::setVisionParamPath(QString path)
{
    set->beginGroup("openvio");
    imagePath = path;
    set->setValue("VISION_PARAM_PATH",path);
    set->endGroup();     
}

QString Setting::getVisionParamPath()
{
    set->beginGroup("openvio");
    QString path = set->value("VISION_PARAM_PATH").toString();
    set->endGroup(); 
    return path;
}

void Setting::setFirmwarePath(QString path)
{
    set->beginGroup("openvio");
    imagePath = path;
    set->setValue("FIRMWARE_PATH",path);
    set->endGroup();     
}

QString Setting::getFirmwarePath()
{
    set->beginGroup("openvio");
    QString path = set->value("FIRMWARE_PATH").toString();
    set->endGroup(); 
    return path;
}

bool Setting::loadVisionParam(QString path)
{
    double p11, p12, p13, p14, p21, p22, p23, p24, p31, p32, p33, p34;
    QSettings *set_vision = new QSettings(path,QSettings::IniFormat); 

    vision_param.CamNum = set_vision->value("CamNum").toInt();

    std::cout << "Load CamNum : " << vision_param.CamNum << std::endl;

    for(int i=0;i<vision_param.CamNum;i++)
    {
        QString P = set_vision->value(QString("P"+QString::number(i))).toString();
        //qDebug() << QString("P"+QString::number(i)) << " = " << P;

        sscanf_s(P.toStdString().data(), "%lf,%lf,%lf,%lf;%lf,%lf,%lf,%lf;%lf,%lf,%lf,%lf", 
        &p11, &p12, &p13, &p14,
        &p21, &p22, &p23, &p24,
        &p31, &p32, &p33, &p34);

        vision_param.P[i] << p11, p12, p13, p14, p21, p22, p23, p24, p31, p32, p33, p34;
        //std::cout << "P" <<i<<":\n"<< vision_param.P[i] << std::endl;

        QString R = set_vision->value(QString("R"+QString::number(i))).toString();
        sscanf_s(R.toStdString().data(), "%lf,%lf,%lf;%lf,%lf,%lf;%lf,%lf,%lf", 
        &p11, &p12, &p13,
        &p21, &p22, &p23,
        &p31, &p32, &p33);

        vision_param.R[i] << p11, p12, p13, p21, p22, p23, p31, p32, p33;
        //std::cout << "R" <<i<<":\n"<< vision_param.R[i] << std::endl;

        QString T = set_vision->value(QString("T"+QString::number(i))).toString();
        sscanf_s(T.toStdString().data(), "%lf,%lf,%lf", 
        &p11, &p12, &p13);

        vision_param.T[i] << p11, p12, p13;
        //std::cout << "T" <<i<<":\n"<< vision_param.T[i] << std::endl;
    }

    vision_param.xy[0].resize(2,vision_param.CamNum);

    return true;
}