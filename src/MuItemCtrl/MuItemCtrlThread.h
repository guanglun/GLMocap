#ifndef MULTEMCTRLTHREAD_H
#define MULTEMCTRLTHREAD_H

#include <QThread>
#include "libusb.h"
#include "workspace.h"



class MuItemCtrlThread : public QThread
{
    Q_OBJECT
private:

public:
    MuItemCtrlThread();
    void run();
signals:

private slots:
 
};

#endif
