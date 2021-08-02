#include "formvisionwindow.h"
#include "ui_formvisionwindow.h"



FormVisionWindow::FormVisionWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormVisionWindow)
{
    ui->setupUi(this);


    
}

FormVisionWindow::~FormVisionWindow()
{
    
    delete ui;
}

void FormVisionWindow::onXYZSlot(double x,double y,double z)
{
    this->ui->lb_x->setText(QString::number(x,10,4));
    this->ui->lb_y->setText(QString::number(y,10,4));
    this->ui->lb_z->setText(QString::number(z,10,4));
}