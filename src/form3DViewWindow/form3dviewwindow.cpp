#include "form3dviewwindow.h"
#include "ui_form3dviewwindow.h"

Form3DViewWindow::Form3DViewWindow(QWidget *parent) : QWidget(parent),
                                                      ui(new Ui::Form3DViewWindow)
{
    ui->setupUi(this);

    camview = new CamView(this);
    ui->verticalLayout->addWidget(camview, 0, 0);

    // view = new GLWidget(this);
    // ui->verticalLayout->addWidget(view,0, 0);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer->start(1000);

    connect(ui->cb_show_model, SIGNAL(stateChanged(int)), this, SLOT(cbShowModelStateChangedSlot(int)));
    connect(ui->cb_show_trajectory, SIGNAL(stateChanged(int)), this, SLOT(cbShowTrajectoryStateChangedSlot(int)));
    connect(ui->cb_show_plan, SIGNAL(stateChanged(int)), this, SLOT(cbShowPlanStateChangedSlot(int)));
    connect(ui->cb_show_axis, SIGNAL(stateChanged(int)), this, SLOT(cbShowAxisStateChangedSlot(int)));
    connect(ui->cb_show_cam, SIGNAL(stateChanged(int)), this, SLOT(cbShowCamStateChangedSlot(int)));


    ui->cb_show_model->setCheckState(Qt::CheckState::Checked);
    ui->cb_show_trajectory->setCheckState(Qt::CheckState::Checked);
    ui->cb_show_plan->setCheckState(Qt::CheckState::Checked);
    ui->cb_show_axis->setCheckState(Qt::CheckState::Checked);
    ui->cb_show_cam->setCheckState(Qt::CheckState::Unchecked);


    camview->cs_show_model = ui->cb_show_model->checkState();
    camview->cs_show_trajectory = ui->cb_show_trajectory->checkState();
    camview->cs_show_plan = ui->cb_show_plan->checkState();
    camview->cs_show_axis = ui->cb_show_axis->checkState();
    camview->cs_show_cam = ui->cb_show_cam->checkState();


}

Form3DViewWindow::~Form3DViewWindow()
{
    delete ui;
}

void Form3DViewWindow::on_pb_reset_trajectory_clicked()
{
    camview->isClearTrajectoryList = true;
}

void Form3DViewWindow::onXYZSlot(Vector3d *Xr, Vector3d *pos, int size)
{
    camview->setPosition(Xr, size,pos);
}

void Form3DViewWindow::onPlanSlot(QList<PlanPoint *> list)
{
    camview->setPlan(list);
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

void Form3DViewWindow::cbShowModelStateChangedSlot(int)
{
    camview->cs_show_model = ui->cb_show_model->checkState();
}
void Form3DViewWindow::cbShowTrajectoryStateChangedSlot(int)
{
    camview->cs_show_trajectory = ui->cb_show_trajectory->checkState();
}
void Form3DViewWindow::cbShowPlanStateChangedSlot(int)
{
    camview->cs_show_plan = ui->cb_show_plan->checkState();
}
void Form3DViewWindow::cbShowAxisStateChangedSlot(int)
{
    camview->cs_show_axis = ui->cb_show_axis->checkState();
}
void Form3DViewWindow::cbShowCamStateChangedSlot(int)
{
    camview->cs_show_cam = ui->cb_show_cam->checkState();
}
