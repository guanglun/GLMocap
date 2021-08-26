#include <QApplication>

#include "formcamwindow.h"
#include "formcvwindow.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FormCamWindow fCamWindow;
    fCamWindow.show();

    return a.exec();
}
