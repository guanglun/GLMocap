#ifndef MULTEMCTRL_H
#define MULTEMCTRL_H

#include "workspace.h"
#include <QObject>
#include <QApplication>
#include <QThread>
#include <QList>
#include <QFileDialog>
#include <QMessageBox>

#include "FirmwareUpgrade.h"

class FormCamWindow;
#include "FormCamWindow.h"

class MuItemCtrlThread;
#include "MuItemCtrlThread.h"

class OPENVIO;
#include "openvio.h"

enum CTRL_TYPE{
    CTRL_TYPE_UPGRADE,
};



class MuItemCtrl : public QObject
{
    Q_OBJECT
private:


public:

    MuItemCtrlThread *muItemCtrlThread;
    OPENVIO *vio;
    QList<OPENVIO*> *vioList;
    enum CTRL_TYPE type;
    FormCamWindow *formCamWindow;
    QString binPath;
    FirmwareUpgrade *firmwareUpgrade;

    MuItemCtrl(FormCamWindow *formCamWindow);
    void setCtrl(enum CTRL_TYPE type,QList<OPENVIO*> *vioList);
    void start();
signals:

private slots:

};

#endif
