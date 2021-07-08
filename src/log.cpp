#include "log.h"

Log::Log()
{

}

void Log::show(QString str)
{
    status_msg->setText(str);
    qDebug() << str;
}
