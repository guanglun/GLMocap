#ifndef USBTHREAD_H
#define USBTHREAD_H

#include <QThread>
#include "libusb.h"
#include "workspace.h"

class WinUSBDriver;
#include "winusbdriver.h"

class USBThread : public QThread
{
    Q_OBJECT
private:
    
    WinUSBDriver *qwindriver;
    bool is_loop;
    QString buikName;
    
public:
    USBThread();
    void init(WinUSBDriver *qwindriver,QString buikName);
    void run();
    void waitClose();
    

};

#endif // USBTHREAD_H
