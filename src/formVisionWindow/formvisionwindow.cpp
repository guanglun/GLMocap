#include "formvisionwindow.h"
#include "ui_formvisionwindow.h"

FormVisionWindow::FormVisionWindow(QWidget *parent) : QWidget(parent),
                                                      ui(new Ui::FormVisionWindow)
{
    ui->setupUi(this);
}

FormVisionWindow::~FormVisionWindow()
{

    delete ui;
}

void FormVisionWindow::onXYZSlot(Vector3d *Xr, Vector3d *pos, int size)
{
    this->ui->lb_x->setText(QString::number(Xr[1](0, 0) / 1000, 10, 2));
    this->ui->lb_y->setText(QString::number(-Xr[1](1, 0) / 1000, 10, 2));
    this->ui->lb_z->setText(QString::number(-Xr[1](2, 0) / 1000, 10, 2));

    this->ui->lb_roll->setText(QString::number(-pos[0](0, 0), 10, 2));
    this->ui->lb_pitch->setText(QString::number(pos[0](1, 0), 10, 2));
    this->ui->lb_yaw->setText(QString::number(pos[0](2, 0), 10, 2));
}