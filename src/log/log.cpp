#include "log.h"

Log::Log()
{

}

void Log::show(QString str,enum LOG_TYPE type)
{
    // if(type == LOG_ALL)
    //     status_msg->setText(str);
    emit logSignal(str);
    qDebug() << str;
}
