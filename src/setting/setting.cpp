#include "setting.h"
#include "MultipleViewTriangulation.h"
#include <iostream>
#include <opencv2/core/eigen.hpp>

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
    getVirtualImagePath();
    getVirtualIndex();
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

void Setting::setVirtualImagePath(QString path)
{
    set->beginGroup("openvio");
    virtualImagePath = path;
    set->setValue("IMAGE_VIRTUAL_PATH", path);
    set->endGroup();
}

QString Setting::getVirtualImagePath()
{
    set->beginGroup("openvio");
    virtualImagePath = set->value("IMAGE_VIRTUAL_PATH").toString();
    set->endGroup();
    return virtualImagePath;
}

int Setting::getVirtualIndex()
{
    set->beginGroup("openvio");
    indexVirtualNumber = set->value("NUMBER_VIRTUAL_INDEX").toInt();
    set->endGroup();
    return indexVirtualNumber;
}

void Setting::setVirtualIndex(int num)
{
    set->beginGroup("openvio");
    indexVirtualNumber = num;
    set->setValue("NUMBER_VIRTUAL_INDEX", num);
    set->endGroup();
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
    if (path.compare(loadVisionParamPath) == 0)
    {
        save_vision = set_vision;
    }
    else
    {
        save_vision = new QSettings(path, QSettings::IniFormat);
    }

    vision_param.CamNum = getCamNumber();
    save_vision->setValue("CamNum", vision_param.CamNum);
    for (int i = 0; i < vision_param.CamNum; i++)
    {
        save_vision->setValue(QString("I" + QString::number(i)),
                            QString::number(vision_param.I[i].row(0)(0), 'f', 15) + "," +
                            QString::number(vision_param.I[i].row(0)(1), 'f', 15) + "," +
                            QString::number(vision_param.I[i].row(0)(2), 'f', 15) + ";" +
                            QString::number(vision_param.I[i].row(1)(0), 'f', 15) + "," +
                            QString::number(vision_param.I[i].row(1)(1), 'f', 15) + "," +
                            QString::number(vision_param.I[i].row(1)(2), 'f', 15) + ";" +
                            QString::number(vision_param.I[i].row(2)(0), 'f', 15) + "," +
                            QString::number(vision_param.I[i].row(2)(1), 'f', 15) + "," +
                            QString::number(vision_param.I[i].row(2)(2), 'f', 15));

        save_vision->setValue(QString("K" + QString::number(i)),
                                QString::number(vision_param.K[i].row(0)(0), 'f', 15) + "," +
                                QString::number(vision_param.K[i].row(0)(1), 'f', 15) + "," +
                                QString::number(vision_param.K[i].row(0)(2), 'f', 15) + "," +
                                QString::number(vision_param.K[i].row(0)(3), 'f', 15) + "," +
                                QString::number(vision_param.K[i].row(0)(4), 'f', 15));

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

        save_vision->setValue(QString("ERR" + QString::number(i)),
                              QString::number(vision_param.ERR[i], 'f', 15));

        vision_param.cameraMatrix[i] = cv::Mat::zeros(3, 3, CV_64F);
        vision_param.distCoeffs[i] = cv::Mat::zeros(3, 3, CV_64F);
        cv::eigen2cv(vision_param.I[i], vision_param.cameraMatrix[i]);
        cv::eigen2cv(vision_param.K[i], vision_param.distCoeffs[i]);
        
        // sprintf(buffer, "%lf,%lf,%lf,%lf;%lf,%lf,%lf,%lf;%lf,%lf,%lf,%lf",
        //          &p11, &p12, &p13, &p14,
        //          &p21, &p22, &p23, &p24,
        //          &p31, &p32, &p33, &p34);
    }
    for (int i = 0; i < vision_param.CamNum-1; i++)
    {
        save_vision->setValue(QString("RMS" + QString::number(i) + "TO" + QString::number(i+1)),
                              QString::number(vision_param.RMS[i], 'f', 15));
    }
    saveGNDVisionParam(save_vision);
}

bool Setting::loadVisionParam(QString path)
{
    double p11, p12, p13, p14, p15, p21, p22, p23, p24, p31, p32, p33, p34;
    loadVisionParamPath = path;
    set_vision = new QSettings(path, QSettings::IniFormat);

    vision_param.CamNum = set_vision->value("CamNum").toInt();

    std::cout << "Load CamNum : " << vision_param.CamNum << std::endl;

    for (int i = 0; i < vision_param.CamNum; i++)
    {
        QString I = set_vision->value(QString("I" + QString::number(i))).toString();
        sscanf_s(I.toStdString().data(), "%lf,%lf,%lf;%lf,%lf,%lf;%lf,%lf,%lf",
                 &p11, &p12, &p13,
                 &p21, &p22, &p23,
                 &p31, &p32, &p33);
        vision_param.I[i] << p11, p12, p13, p21, p22, p23, p31, p32, p33;
        std::cout << "I" << i << ":\n"
                  << vision_param.I[i] << std::endl;

        QString K = set_vision->value(QString("K" + QString::number(i))).toString();
        sscanf_s(K.toStdString().data(), "%lf,%lf,%lf,%lf,%lf",
                 &p11, &p12, &p13, &p14, &p15);
        vision_param.K[i] << p11, p12, p13, p14, p15;
        std::cout << "K" << i << ":\n"
                  << vision_param.K[i] << std::endl;

        vision_param.cameraMatrix[i] = cv::Mat::zeros(3, 3, CV_64F);
        vision_param.distCoeffs[i] = cv::Mat::zeros(3, 3, CV_64F);
        cv::eigen2cv(vision_param.I[i], vision_param.cameraMatrix[i]);
        cv::eigen2cv(vision_param.K[i], vision_param.distCoeffs[i]);

        std::cout << "I" << i << ":\n"
                  << vision_param.cameraMatrix[i] << std::endl;

        std::cout << "K" << i << ":\n"
                  << vision_param.distCoeffs[i] << std::endl;

        vision_param.P[i] << p11, p12, p13, p14, p21, p22, p23, p24, p31, p32, p33, p34;

        QString P = set_vision->value(QString("P" + QString::number(i))).toString();
        //qDebug() << QString("P"+QString::number(i)) << " = " << P;

        sscanf_s(P.toStdString().data(), "%lf,%lf,%lf,%lf;%lf,%lf,%lf,%lf;%lf,%lf,%lf,%lf",
                 &p11, &p12, &p13, &p14,
                 &p21, &p22, &p23, &p24,
                 &p31, &p32, &p33, &p34);

        vision_param.P[i] << p11, p12, p13, p14, p21, p22, p23, p24, p31, p32, p33, p34;
        // std::cout << "P" << i << ":\n"
        //           << vision_param.P[i] << std::endl;

        QString R = set_vision->value(QString("R" + QString::number(i))).toString();
        //mlog->show(R);
        sscanf_s(R.toStdString().data(), "%lf,%lf,%lf;%lf,%lf,%lf;%lf,%lf,%lf",
                 &p11, &p12, &p13,
                 &p21, &p22, &p23,
                 &p31, &p32, &p33);
        Matrix33d R_;
        R_ << p11, p12, p13, p21, p22, p23, p31, p32, p33;
        vision_param.R[i] = R_;
        // std::cout << "R" << i << ":\n"
        //           << vision_param.R[i] << std::endl;

        QString T = set_vision->value(QString("T" + QString::number(i))).toString();
        //mlog->show(T);
        sscanf_s(T.toStdString().data(), "%lf;%lf;%lf",
                 &p11, &p12, &p13);
        RowVector3d T_;
        T_ << p11, p12, p13;
        vision_param.T[i] = T_;
        // std::cout << "T" << i << ":\n"
        //           << vision_param.T[i] << std::endl;
    }

    QString RGND = set_vision->value("RGND").toString();
    sscanf_s(RGND.toStdString().data(), "%lf,%lf,%lf;%lf,%lf,%lf;%lf,%lf,%lf",
             &p11, &p12, &p13,
             &p21, &p22, &p23,
             &p31, &p32, &p33);
    vision_param.RGND << p11, p12, p13, p21, p22, p23, p31, p32, p33;

    QString TGND = set_vision->value("TGND").toString();
    sscanf_s(TGND.toStdString().data(), "%lf;%lf;%lf",
             &p11, &p12, &p13);
    vision_param.TGND << p11, p12, p13;

    Matrix44d RTGND;
    RTGND << vision_param.RGND(0, 0), vision_param.RGND(1, 0), vision_param.RGND(2, 0), vision_param.TGND(0, 0),
        vision_param.RGND(0, 1), vision_param.RGND(1, 1), vision_param.RGND(2, 1), vision_param.TGND(1, 0),
        vision_param.RGND(0, 2), vision_param.RGND(1, 2), vision_param.RGND(2, 2), vision_param.TGND(2, 0),
        0, 0, 0, 1;

    Matrix33d RR = vision_param.RGND; //.transpose();
    Vector3d TT = -vision_param.TGND;
    vision_param.RTGND = EasyTool::getRT44d(RR, TT);
    vision_param.RTGNDINV = RTGND.inverse();
    vision_param.eulerAngles = vision_param.RGND.transpose().eulerAngles(0, 1, 2);

    vision_param.xy[0].resize(2, vision_param.CamNum);
    vision_param.idx.resize(PT_NUM_MAX, CAM_NUM_MAX);

    return true;
}

void Setting::saveGNDVisionParam(QSettings *setting)
{
    QSettings *tar_setting;
    if (setting == nullptr)
    {
        tar_setting = set_vision;
    }
    else
    {
        tar_setting = setting;
    }

    tar_setting->setValue("RGND",
                          QString::number(vision_param.RGND(0, 0), 'f', 15) + "," +
                              QString::number(vision_param.RGND(0, 1), 'f', 15) + "," +
                              QString::number(vision_param.RGND(0, 2), 'f', 15) + ";" +
                              QString::number(vision_param.RGND(1, 0), 'f', 15) + "," +
                              QString::number(vision_param.RGND(1, 1), 'f', 15) + "," +
                              QString::number(vision_param.RGND(1, 2), 'f', 15) + ";" +
                              QString::number(vision_param.RGND(2, 0), 'f', 15) + "," +
                              QString::number(vision_param.RGND(2, 1), 'f', 15) + "," +
                              QString::number(vision_param.RGND(2, 2), 'f', 15));

    tar_setting->setValue("TGND", QString::number(vision_param.TGND(0, 0), 'f', 15) + ";" +
                                      QString::number(vision_param.TGND(1, 0), 'f', 15) + ";" +
                                      QString::number(vision_param.TGND(2, 0), 'f', 15));
}