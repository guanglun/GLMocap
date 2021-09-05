#include "formpx4window.h"
#include "ui_formpx4window.h"

FormPx4Window::FormPx4Window(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::FormPx4Window)
{
    ui->setupUi(this);
    //updatePosText();
}

FormPx4Window::~FormPx4Window()
{
    delete ui;
}

void FormPx4Window::setPX4Thread(PX4Thread *px4)
{
    this->px4 = px4;
}

void FormPx4Window::on_pb_ini_mavlink_clicked()
{
    px4->initMavlink();
}

void FormPx4Window::on_pb_disarm_clicked()
{
    px4->setArm(0);
}

void FormPx4Window::on_pb_arm_clicked()
{
    px4->setTargetPosition(0,0,0,0);
    px4->setArm(1);
}

void FormPx4Window::on_pb_take_off_clicked()
{
    px4->setTargetPosition(0,0,0.5,0);
    updatePosText();
}

void FormPx4Window::on_pb_land_clicked()
{
    px4->setTargetPosition(0,0,0,0);
    updatePosText();
}

void FormPx4Window::on_pb_set_position_clicked()
{
    float x = (float)ui->sb_x->value()/1000.0;
    float y = (float)ui->sb_y->value()/1000.0;
    float z = (float)ui->sb_z->value()/1000.0;
    float yaw = (float)ui->sb_yaw->value()/1000.0;

    px4->setTargetPosition(x,y,z,yaw);
    
    updatePosText();
}

void FormPx4Window::updatePosText()
{
    QString str = "position: " +
                        QString::number(px4->tar_x, 'f', 2) + "  " +
                        QString::number(px4->tar_y, 'f', 2) + "  " +
                        QString::number(px4->tar_z, 'f', 2) + "  " +
                        QString::number(px4->tar_yaw, 'f', 2);

    ui->lb_position->setText(str);

    mlog->show(str);
}