#include "form3dviewwindow.h"
#include "ui_form3dviewwindow.h"



Form3DViewWindow::Form3DViewWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form3DViewWindow)
{
    ui->setupUi(this);


    camview = new CamView(this);
    ui->verticalLayout->addWidget(camview,0,0);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer->start(1000);    
}

Form3DViewWindow::~Form3DViewWindow()
{
    
    delete ui;
}

void Form3DViewWindow::onXYZSlot(double x,double y,double z)
{
    camview->setPosition(x/100,y/100,z/100);
}

void Form3DViewWindow::onTimeOut()
{
    QString speedStr = "view : ";
    speedStr += QString::number(camview->view_fps_1s);
    camview->view_fps_1s = 0;
    speedStr += "fps";
    ui->lb_3dview_fps->setText(speedStr);

    speedStr = "point : ";
    speedStr += QString::number(camview->point_fps_1s);
    camview->point_fps_1s = 0;
    speedStr += "fps";
    ui->lb_point_fps->setText(speedStr);
}
