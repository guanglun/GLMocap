#ifndef PX4THREAD_H
#define PX4THREAD_H

#include <QThread>
#include "workspace.h"

class PX4Thread : public QThread
{
    Q_OBJECT
private:

public:
    PX4Thread();
    void run();
signals:

private slots:
 
};

#endif
