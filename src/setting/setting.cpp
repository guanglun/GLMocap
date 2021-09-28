#include "setting.h"
#include "MultipleViewTriangulation.h"
#include <iostream>

Setting::Setting()
{
    QApplication::setOrganizationName("openvio");
    QApplication::setOrganizationDomain("openvio.com");
    QApplication::setApplicationName("openvio");

    QString setFile = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/.settings.ini";
    qDebug() << "configure file path:" << setFile;
    set = new QSettings(setFile, QSettings::IniFormat);

    set->beginGroup("openvio");
    QString thr = set->value(VISION_THRESHOLD.SET_NAME).toString();
    if (thr.isEmpty())
    {
        set->setValue(VISION_THRESHOLD.SET_NAME, VISION_THRESHOLD.DEFAULT_VALUE);
        threshold = VISION_THRESHOLD.DEFAULT_VALUE.toInt();
    }
    set->endGroup();

    getCamNumber();
    getThreshold();
    getImagePath();
    getCalibrPath();
}

void Setting::getIMUOffset(T_int16_xyz *acc, T_int16_xyz *gyro)
{
    set->beginGroup("openvio");

    QString acc_offset_x = set->value(ACC_OFFSET_X.SET_NAME).toString();
    QString acc_offset_y = set->value(ACC_OFFSET_Y.SET_NAME).toString();
    QString acc_offset_z = set->value(ACC_OFFSET_Z.SET_NAME).toString();

    QString gyr_offset_x = set->value(GYR_OFFSET_X.SET_NAME).toString();
    QString gyr_offset_y = set->value(GYR_OFFSET_Y.SET_NAME).toString();
    QString gyr_offset_z = set->value(GYR_OFFSET_Z.SET_NAME).toString();

    if (acc_offset_x.isEmpty())
    {
        set->setValue(ACC_OFFSET_X.SET_NAME, ACC_OFFSET_X.DEFAULT_VALUE);
        acc_offset_x = ACC_OFFSET_X.DEFAULT_VALUE;
    }
    acc->X = acc_offset_x.toInt();

    if (acc_offset_y.isEmpty())
    {
        set->setValue(ACC_OFFSET_Y.SET_NAME, ACC_OFFSET_Y.DEFAULT_VALUE);
        acc_offset_y = ACC_OFFSET_Y.DEFAULT_VALUE;
    }
    acc->Y = acc_offset_y.toInt();

    if (acc_offset_z.isEmpty())
    {
        set->setValue(ACC_OFFSET_Z.SET_NAME, ACC_OFFSET_Z.DEFAULT_VALUE);
        acc_offset_z = ACC_OFFSET_Z.DEFAULT_VALUE;
    }
    acc->Z = acc_offset_z.toInt();

    if (gyr_offset_x.isEmpty())
    {
        set->setValue(GYR_OFFSET_X.SET_NAME, GYR_OFFSET_X.DEFAULT_VALUE);
        gyr_offset_x = GYR_OFFSET_X.DEFAULT_VALUE;
    }
    gyro->X = gyr_offset_x.toInt();

    if (gyr_offset_y.isEmpty())
    {
        set->setValue(GYR_OFFSET_Y.SET_NAME, GYR_OFFSET_Y.DEFAULT_VALUE);
        gyr_offset_y = GYR_OFFSET_Y.DEFAULT_VALUE;
    }
    gyro->Y = gyr_offset_y.toInt();

    if (gyr_offset_z.isEmpty())
    {
        set->setValue(GYR_OFFSET_Z.SET_NAME, GYR_OFFSET_Z.DEFAULT_VALUE);
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
    set->setValue(ACC_OFFSET_X.SET_NAME, QString::number(acc->X));
    set->setValue(ACC_OFFSET_Y.SET_NAME, QString::number(acc->Y));
    set->setValue(ACC_OFFSET_Z.SET_NAME, QString::number(acc->Z));
    set->endGroup();
}

void Setting::setGyroOffset(T_int16_xyz *gyro)
{
    set->beginGroup("openvio");
    set->setValue(GYR_OFFSET_X.SET_NAME, QString::number(gyro->X));
    set->setValue(GYR_OFFSET_Y.SET_NAME, QString::number(gyro->Y));
    set->setValue(GYR_OFFSET_Z.SET_NAME, QString::number(gyro->Z));
    set->endGroup();
}

void Setting::setNumberById(QString id, int number)
{
    set->beginGroup("openvio");
    set->setValue(id, number);
    set->endGroup();
}

int Setting::getNumberById(QString id)
{
    int number = -1;
    set->beginGroup("openvio");
    if (set->contains(id))
        number = set->value(id).toInt();
    set->endGroup();
    return number;
}

void Setting::setImagePath(QString path)
{
    set->beginGroup("openvio");
    imagePath = path;
    set->setValue("IMAGE_PATH", path);
    set->endGroup();
}

QString Setting::getImagePath()
{
    set->beginGroup("openvio");
    imagePath = set->value("IMAGE_PATH").toString();
    set->endGroup();
    return imagePath;
}

void Setting::setCalibrPath(QString path)
{
    set->beginGroup("openvio");
    calibrPath = path;
    set->setValue("CALIBR_PATH", path);
    set->endGroup();
}

QString Setting::getCalibrPath()
{
    set->beginGroup("openvio");
    calibrPath = set->value("CALIBR_PATH").toString();
    set->endGroup();
    return calibrPath;
}

int Setting::getThreshold()
{
    set->beginGroup("openvio");
    threshold = set->value("VISION_THRESHOLD").toInt();
    set->endGroup();
    return threshold;
}

void Setting::setThreshold(int thr)
{
    set->beginGroup("openvio");
    threshold = thr;
    set->setValue("VISION_THRESHOLD", thr);
    set->endGroup();
}

int Setting::getCamNumber()
{
    set->beginGroup("openvio");
    camNumber = set->value("NUMBER_CAMS").toInt();
    set->endGroup();
    return camNumber;
}

void Setting::setCamNumber(int num)
{
    set->beginGroup("openvio");
    camNumber = num;
    set->setValue("NUMBER_CAMS", num);
    set->endGroup();
}

void Setting::setVisionParamPath(QString path)
{
    set->beginGroup("openvio");
    set->setValue("VISION_PARAM_PATH", path);
    set->endGroup();
}

QString Setting::getVisionParamPath()
{
    set->beginGroup("openvio");
    QString path = set->value("VISION_PARAM_PATH").toString();
    set->endGroup();
    return path;
}

void Setting::setSaveVisionParamPath(QString path)
{
    set->beginGroup("openvio");
    set->setValue("VISION_SAVE_PARAM_PATH", path);
    set->endGroup();
}

QString Setting::getSaveVisionParamPath()
{
    set->beginGroup("openvio");
    QString path = set->value("VISION_SAVE_PARAM_PATH").toString();
    set->endGroup();
    return path;
}

void Setting::setFirmwarePath(QString path)
{
    set->beginGroup("openvio");
    imagePath = path;
    set->setValue("FIRMWARE_PATH", path);
    set->endGroup();
}

QString Setting::getFirmwarePath()
{
    set->beginGroup("openvio");
    QString path = set->value("FIRMWARE_PATH").toString();
    set->endGroup();
    return path;
}

bool Setting::saveVisionParam(QString path)
{
    char buffer[2048];
    save_vision = new QSettings(path, QSettings::IniFormat);

    save_vision->setValue("CamNum", vision_param.CamNum);
    for (int i = 0; i < vision_param.CamNum; i++)
    {
        save_vision->setValue(QString("P" + QString::number(i)),
                              QString::number(vision_param.P[i].row(0)(0), 'f', 15) + "," +
                                  QString::number(vision_param.P[i].row(0)(1), 'f', 15) + "," +
                                  QString::number(vision_param.P[i].row(0)(2), 'f', 15) + "," +
                                  QString::number(vision_param.P[i].row(0)(3), 'f', 15) + ";" +
                                  QString::number(vision_param.P[i].row(1)(0), 'f', 15) + "," +
                                  QString::number(vision_param.P[i].row(1)(1), 'f', 15) + "," +
                                  QString::number(vision_param.P[i].row(1)(2), 'f', 15) + "," +
                                  QString::number(vision_param.P[i].row(1)(3), 'f', 15) + ";" +
                                  QString::number(vision_param.P[i].row(2)(0), 'f', 15) + "," +
                                  QString::number(vision_param.P[i].row(2)(1), 'f', 15) + "," +
                                  QString::number(vision_param.P[i].row(2)(2), 'f', 15) + "," +
                                  QString::number(vision_param.P[i].row(2)(3), 'f', 15));

        save_vision->setValue(QString("R" + QString::number(i)),
                              QString::number(vision_param.R[i].row(0)(0), 'f', 15) + "," +
                                  QString::number(vision_param.R[i].row(0)(1), 'f', 15) + "," +
                                  QString::number(vision_param.R[i].row(0)(2), 'f', 15) + ";" +
                                  QString::number(vision_param.R[i].row(1)(0), 'f', 15) + "," +
                                  QString::number(vision_param.R[i].row(1)(1), 'f', 15) + "," +
                                  QString::number(vision_param.R[i].row(1)(2), 'f', 15) + ";" +
                                  QString::number(vision_param.R[i].row(2)(0), 'f', 15) + "," +
                                  QString::number(vision_param.R[i].row(2)(1), 'f', 15) + "," +
                                  QString::number(vision_param.R[i].row(2)(2), 'f', 15));

        save_vision->setValue(QString("T" + QString::number(i)),
                              QString::number(vision_param.T[i].row(0)(0), 'f', 15) + ";" +
                                  QString::number(vision_param.T[i].row(1)(0), 'f', 15) + ";" +
                                  QString::number(vision_param.T[i].row(2)(0), 'f', 15));
        // sprintf(buffer, "%lf,%lf,%lf,%lf;%lf,%lf,%lf,%lf;%lf,%lf,%lf,%lf",
        //          &p11, &p12, &p13, &p14,
        //          &p21, &p22, &p23, &p24,
        //          &p31, &p32, &p33, &p34);
    }

    save_vision->setValue("RGND",
                          QString::number(vision_param.RGND(0, 0)) + "," +
                              QString::number(vision_param.RGND(0, 1), 'f', 15) + "," +
                              QString::number(vision_param.RGND(0, 2), 'f', 15) + ";" +
                              QString::number(vision_param.RGND(1, 0), 'f', 15) + "," +
                              QString::number(vision_param.RGND(1, 1), 'f', 15) + "," +
                              QString::number(vision_param.RGND(1, 2), 'f', 15) + ";" +
                              QString::number(vision_param.RGND(2, 0), 'f', 15) + "," +
                              QString::number(vision_param.RGND(2, 1), 'f', 15) + "," +
                              QString::number(vision_param.RGND(2, 2), 'f', 15));

    save_vision->setValue("TGND", QString::number(vision_param.TGND(0, 0), 'f', 15) + "," +
                                      QString::number(vision_param.TGND(0, 1), 'f', 15) + "," +
                                      QString::number(vision_param.TGND(0, 2), 'f', 15));
}

bool Setting::loadVisionParam(QString path)
{
    double p11, p12, p13, p14, p21, p22, p23, p24, p31, p32, p33, p34;
    set_vision = new QSettings(path, QSettings::IniFormat);

    vision_param.CamNum = set_vision->value("CamNum").toInt();

    std::cout << "Load CamNum : " << vision_param.CamNum << std::endl;

    for (int i = 0; i < vision_param.CamNum; i++)
    {
        QString P = set_vision->value(QString("P" + QString::number(i))).toString();
        //qDebug() << QString("P"+QString::number(i)) << " = " << P;

        sscanf_s(P.toStdString().data(), "%lf,%lf,%lf,%lf;%lf,%lf,%lf,%lf;%lf,%lf,%lf,%lf",
                 &p11, &p12, &p13, &p14,
                 &p21, &p22, &p23, &p24,
                 &p31, &p32, &p33, &p34);

        vision_param.P[i] << p11, p12, p13, p14, p21, p22, p23, p24, p31, p32, p33, p34;
        //std::cout << "P" <<i<<":\n"<< vision_param.P[i] << std::endl;

        QString R = set_vision->value(QString("R" + QString::number(i))).toString();
        //mlog->show(R);
        sscanf_s(R.toStdString().data(), "%lf,%lf,%lf;%lf,%lf,%lf;%lf,%lf,%lf",
                 &p11, &p12, &p13,
                 &p21, &p22, &p23,
                 &p31, &p32, &p33);
        Matrix33d R_;
        R_ << p11, p12, p13, p21, p22, p23, p31, p32, p33;
        vision_param.R[i] = R_;
        std::cout << "R" << i << ":\n"
                  << vision_param.R[i] << std::endl;

        QString T = set_vision->value(QString("T" + QString::number(i))).toString();
        //mlog->show(T);
        sscanf_s(T.toStdString().data(), "%lf;%lf;%lf",
                 &p11, &p12, &p13);
        RowVector3d T_;
        T_ << p11, p12, p13;
        vision_param.T[i] = T_;
        std::cout << "T" << i << ":\n"
                  << vision_param.T[i] << std::endl;
    }

    QString RGND = set_vision->value("RGND").toString();
    sscanf_s(RGND.toStdString().data(), "%lf,%lf,%lf;%lf,%lf,%lf;%lf,%lf,%lf",
             &p11, &p12, &p13,
             &p21, &p22, &p23,
             &p31, &p32, &p33);
    vision_param.RGND << p11, p12, p13, p21, p22, p23, p31, p32, p33;

    QString TGND = set_vision->value("TGND").toString();
    sscanf_s(TGND.toStdString().data(), "%lf,%lf,%lf",
             &p11, &p12, &p13);
    vision_param.TGND << p11, p12, p13;

    Matrix44d RTGND;
    RTGND << vision_param.RGND(0, 0), vision_param.RGND(1, 0), vision_param.RGND(2, 0), vision_param.TGND(0, 0),
        vision_param.RGND(0, 1), vision_param.RGND(1, 1), vision_param.RGND(2, 1), vision_param.TGND(0, 1),
        vision_param.RGND(0, 2), vision_param.RGND(1, 2), vision_param.RGND(2, 2), vision_param.TGND(0, 2),
        0, 0, 0, 1;

    Matrix33d RR = vision_param.RGND;//.transpose();
    Vector3d  TT = -vision_param.TGND;
    vision_param.RTGND = EasyTool::getRT44d(RR,TT);
    vision_param.RTGNDINV = RTGND.inverse();
    vision_param.eulerAngles = vision_param.RGND.transpose().eulerAngles(0, 1, 2);

    vision_param.xy[0].resize(2, vision_param.CamNum);
    vision_param.idx.resize(PT_NUM_MAX, CAM_NUM_MAX);
    return true;
}

void Setting::saveGNDVisionParam(void)
{
    set_vision->setValue("RGND",
                         QString::number(vision_param.RGND(0, 0), 'f', 15) + "," +
                             QString::number(vision_param.RGND(0, 1), 'f', 15) + "," +
                             QString::number(vision_param.RGND(0, 2), 'f', 15) + ";" +
                             QString::number(vision_param.RGND(1, 0), 'f', 15) + "," +
                             QString::number(vision_param.RGND(1, 1), 'f', 15) + "," +
                             QString::number(vision_param.RGND(1, 2), 'f', 15) + ";" +
                             QString::number(vision_param.RGND(2, 0), 'f', 15) + "," +
                             QString::number(vision_param.RGND(2, 1), 'f', 15) + "," +
                             QString::number(vision_param.RGND(2, 2), 'f', 15));

    set_vision->setValue("TGND", QString::number(vision_param.TGND(0, 0), 'f', 15) + "," +
                                     QString::number(vision_param.TGND(0, 1), 'f', 15) + "," +
                                     QString::number(vision_param.TGND(0, 2), 'f', 15));
}