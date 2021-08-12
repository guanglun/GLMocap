#ifndef MULTEMCTRL_H
#define MULTEMCTRL_H

#include "workspace.h"
#include <QObject>
#include <QApplication>
#include <QThread>

class OPENVIO;
#include "openvio.h"


class MuItemCtrl : public QObject
{
    Q_OBJECT
private:


public:
    OPENVIO *vio;

    MuItemCtrl();

signals:

private slots:

};

#endif
