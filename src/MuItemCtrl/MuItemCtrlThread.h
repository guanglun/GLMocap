#ifndef MULTEMCTRLTHREAD_H
#define MULTEMCTRLTHREAD_H

#include <QThread>
#include "libusb.h"
#include "workspace.h"
#include "FirmwareUpgrade.h"

class MuItemCtrl;
#include "MuItemCtrl.h"

class OPENVIO;
#include "openvio.h"

class MuItemCtrlThread : public QThread
{
    Q_OBJECT
private:
    QList<QString> idlist;
    MuItemCtrl *muItemCtrl;

public:
    MuItemCtrlThread(MuItemCtrl *muItemCtrl);
    void run();
    int upgrade(OPENVIO *vio);
signals:

private slots:
 
};

#endif
