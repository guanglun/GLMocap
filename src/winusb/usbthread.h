#ifndef USBTHREAD_H
#define USBTHREAD_H

#include <QThread>
#include "libusb.h"
#include "workspace.h"


class OPENVIO;
#include "openvio.h"

class WinUSBDriver;
#include "winusbdriver.h"

class USBThread : public QThread
{
    Q_OBJECT
private:
    
    OPENVIO *vio;
    bool is_loop;
    QString buikName;
    
public:
    USBThread();
    void init(OPENVIO *vio,QString buikName);
    void run();
    void waitClose();
    

};

#endif // USBTHREAD_H
