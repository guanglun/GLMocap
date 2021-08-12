#ifndef LOG_H
#define LOG_H


#include <QObject>
#include <QDebug>
#include "ui_mainwindow.h"

extern QLabel *status_msg;

enum LOG_TYPE{
    LOG_ALL,
    LOG_DEBUG
};

class Log : public QObject
{
    Q_OBJECT
public:
    
    
    Log();
    void show(QString str,enum LOG_TYPE type = LOG_ALL);
private slots:

signals:
    void logSignal(QString msg);
};

#endif // LOG_H
