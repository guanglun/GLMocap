#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "workspace.h"

#include "MultipleViewTriangulation.h"

class Calibration : public QObject
{
    Q_OBJECT

private:
    QStringList checkFile(QString path);
public:
    Calibration();
    void calibrStart(QString path);
    void msg(QString msg);
signals:
    void logSignal(QString msg);
};

#endif //CALIBRATION_H
