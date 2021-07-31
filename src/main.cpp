#include "mainwindow.h"
#include <QApplication>

#include "formcamwindow.h"
#include "formcvwindow.h"

#include "MultipleViewTriangulation.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;
    // w.show();
    
    FormCamWindow fCamWindow;
    fCamWindow.show();

    MultipleViewTriangulation multipleViewTriangulation;

    return a.exec();
}
