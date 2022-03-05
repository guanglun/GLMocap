#include "formabout.h"
#include "ui_formabout.h"
#include <QFileDialog>

FormAbout::FormAbout(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::FormAbout)
{
    ui->setupUi(this);

    this->setWindowTitle("about");

    ui->lb_version->setText("VERSION : " + QString(VERSION));
}

FormAbout::~FormAbout()
{
    delete ui;
}

void FormAbout::on_pb_openlink_clicked()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("http://guanglundz.com/mocap")));
}
