#include "form3dviewwindow.h"
#include "ui_form3dviewwindow.h"



Form3DViewWindow::Form3DViewWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form3DViewWindow)
{
    ui->setupUi(this);


    camview = new CamView(this);
    ui->verticalLayout->addWidget(camview,0,0);
}

Form3DViewWindow::~Form3DViewWindow()
{
    
    delete ui;
}

void Form3DViewWindow::onXYZSlot(double x,double y,double z)
{
    camview->setPosition(x/100,y/100,z/100);
}