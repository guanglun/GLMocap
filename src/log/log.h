#ifndef LOG_H
#define LOG_H


#include <QObject>
#include <QDebug>
#include "ui_mainwindow.h"

extern QLabel *status_msg;

class Log : public QObject
{
    Q_OBJECT
public:
    
    
    Log();
    void show(QString str);
};

#endif // LOG_H
