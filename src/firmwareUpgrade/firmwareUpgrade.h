#ifndef FIRMWAREUPGRADE_H
#define FIRMWAREUPGRADE_H

#include "workspace.h"
#include <QObject>
#include <QApplication>

class OPENVIO;
#include "openvio.h"


class FirmwareUpgrade : public QObject
{
    Q_OBJECT
private:
    OPENVIO *vio;
public:
    FirmwareUpgrade(OPENVIO *vio);
    void upgradeStart();
signals:
    //void camSignals(int index);
    //void imuSignals(int index);   
    //void closeSignals(void);   
private slots:
    //void closeSlot(void);    
};

#endif
