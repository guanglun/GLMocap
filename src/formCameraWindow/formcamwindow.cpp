#include "formcamwindow.h"
#include "ui_formcamwindow.h"

FormCamWindow::FormCamWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCamWindow)
{
    ui->setupUi(this);
    
}

FormCamWindow::~FormCamWindow()
{
    delete ui;
}
