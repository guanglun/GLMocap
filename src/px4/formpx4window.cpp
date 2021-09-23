#include "formpx4window.h"
#include "ui_formpx4window.h"

#include "MuItemPoint.h"

FormPx4Window::FormPx4Window(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::FormPx4Window)
{
    ui->setupUi(this);

    pModelPlan = new QStandardItemModel();
    MuItemPoint *pItemPoint = new MuItemPoint(this);

    ui->lv_plan->setItemDelegate(pItemPoint);
    ui->lv_plan->setModel(pModelPlan);
    ui->lv_plan->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lv_plan->setContextMenuPolicy(Qt::CustomContextMenu);

    qRegisterMetaType<QItemSelection>("QItemSelection");
    qRegisterMetaType<PLAN_POINT_STATE>("PLAN_POINT_STATE");
    planFlyProcess = new PlanFlyProcess(this);
    planFlyProcess->setQListView(ui->lv_plan);
    planFlyProcess->setModelPlan(pModelPlan);
    planFlyProcess->moveToThread(&planFlyProcessThread);
    planFlyProcessThread.start();

    connect(this, SIGNAL(startPlanSignal()), planFlyProcess, SLOT(startSlot()));
}

#define COUNT 10
#define WAIT_TIME 200
void FormPx4Window::on_pb_load_plan1_clicked()
{
    pModelPlan->clear();
    ppList.clear();

    ppList.append(new PlanPoint(0, 0, 0.5));

    for(int i=1;i<COUNT;i++)
    {
        ppList.append(new PlanPoint(-0.02*i, -0.02*i, 0.5,WAIT_TIME));
    }
    ppList.append(new PlanPoint(-0.2, -0.2, 0.5,WAIT_TIME*6));

    for(int i=1;i<COUNT;i++)
    {
        ppList.append(new PlanPoint(-0.2, -0.2 + 0.02*i*2, 0.5,WAIT_TIME));
    }
    ppList.append(new PlanPoint(-0.2, 0.2, 0.5,WAIT_TIME*6));

    for(int i=1;i<COUNT;i++)
    {
        ppList.append(new PlanPoint(-0.2 + 0.02*i*2, 0.2, 0.5,WAIT_TIME));
    }
    ppList.append(new PlanPoint(0.2, 0.2, 0.5,WAIT_TIME*6));

    for(int i=1;i<COUNT;i++)
    {
        ppList.append(new PlanPoint(0.2, 0.2 - 0.02*i*2, 0.5,WAIT_TIME));
    }
    ppList.append(new PlanPoint(0.2, -0.2, 0.5,WAIT_TIME*6));

    for(int i=1;i<COUNT;i++)
    {
        ppList.append(new PlanPoint(0.2 - 0.02*i*2, -0.2, 0.5,WAIT_TIME));
    }
    ppList.append(new PlanPoint(-0.2, -0.2, 0.5,WAIT_TIME*6));

    for(int i=1;i<COUNT;i++)
    {
        ppList.append(new PlanPoint(-0.2 + 0.02*i, -0.2 + 0.02*i, 0.5,WAIT_TIME));
    }
    ppList.append(new PlanPoint(0, 0, 0.5,WAIT_TIME*6));

    for (int i = 0; i < ppList.size(); i++)
    { 
        pModelPlan->appendRow(ppList.at(i)->item);
        
    }

    emit onPlanSignals(ppList);
}
#define PLAN2_COUNT (4*4*4)
void FormPx4Window::on_pb_load_plan2_clicked()
{
    pModelPlan->clear();
    ppList.clear();

    ppList.append(new PlanPoint(0, 0, 0.5));
    for(int i=1;i<10;i++)
    {
        ppList.append(new PlanPoint(0, 0.03*i, 0.5,WAIT_TIME));
    }
    ppList.append(new PlanPoint(0, 0.3, 0.5,WAIT_TIME*6));

    float x,y;
    float R=0.3;

    for(int i=0;i<PLAN2_COUNT+1;i++)
    {
        x = R*sin(360.0f/PLAN2_COUNT*i*M_PI/180);
        y = R*cos(360.0f/PLAN2_COUNT*i*M_PI/180);
        ppList.append(new PlanPoint(x, y, 0.5,WAIT_TIME));
    }

    ppList.append(new PlanPoint(0, 0.3, 0.5,WAIT_TIME*6));
    for(int i=1;i<10;i++)
    {
        ppList.append(new PlanPoint(0, 0.3 - 0.03*i, 0.5,WAIT_TIME));
    }
    ppList.append(new PlanPoint(0, 0, 0.5,WAIT_TIME*6));

    for (int i = 0; i < ppList.size(); i++)
    { 
        pModelPlan->appendRow(ppList.at(i)->item);
        
    }

    emit onPlanSignals(ppList);
}

void FormPx4Window::on_pb_load_plan3_clicked()
{
    pModelPlan->clear();
    ppList.clear();

    ppList.append(new PlanPoint(0, 0, 0.3));
    for(int i=1;i<10;i++)
    {
        ppList.append(new PlanPoint(0, 0.02*i, 0.3,WAIT_TIME));
    }
    ppList.append(new PlanPoint(0, 0.2, 0.3,WAIT_TIME*6));

    float x,y;
    float R=0.2;

    for(int i=0;i<PLAN2_COUNT+1;i++)
    {
        x = R*sin(360.0f/PLAN2_COUNT*i*M_PI/180);
        y = R*cos(360.0f/PLAN2_COUNT*i*M_PI/180);
        ppList.append(new PlanPoint(x, y, 0.3 + 0.2f/PLAN2_COUNT*i,WAIT_TIME));
    }
    for(int i=1;i<PLAN2_COUNT+1;i++)
    {
        x = R*sin(360.0f/PLAN2_COUNT*i*M_PI/180);
        y = R*cos(360.0f/PLAN2_COUNT*i*M_PI/180);
        ppList.append(new PlanPoint(x, y, 0.5 + 0.2f/PLAN2_COUNT*i,WAIT_TIME));
    }

    for(int i=1;i<PLAN2_COUNT+1;i++)
    {
        x = R*sin(360.0f/PLAN2_COUNT*i*M_PI/180);
        y = R*cos(360.0f/PLAN2_COUNT*i*M_PI/180);
        ppList.append(new PlanPoint(x, y, 0.7 - 0.2f/PLAN2_COUNT*i,WAIT_TIME));
    }

    for(int i=1;i<PLAN2_COUNT;i++)
    {
        x = R*sin(360.0f/PLAN2_COUNT*i*M_PI/180);
        y = R*cos(360.0f/PLAN2_COUNT*i*M_PI/180);
        ppList.append(new PlanPoint(x, y, 0.5 - 0.2f/PLAN2_COUNT*i,WAIT_TIME));
    }

    ppList.append(new PlanPoint(0, 0.2, 0.3,WAIT_TIME*6));

    for(int i=1;i<10;i++)
    {
        ppList.append(new PlanPoint(0, 0.2 - 0.02*i, 0.3,WAIT_TIME));
    }

    ppList.append(new PlanPoint(0, 0, 0.3,WAIT_TIME*6));

    for (int i = 0; i < ppList.size(); i++)
    { 
        pModelPlan->appendRow(ppList.at(i)->item);
    }

    emit onPlanSignals(ppList);
}

void FormPx4Window::on_pb_start_plan_clicked()
{

    planFlyProcess->setPlan(ppList);
    emit startPlanSignal();
}
void FormPx4Window::on_pb_stop_plan_clicked()
{
    planFlyProcess->stop();
}

FormPx4Window::~FormPx4Window()
{
    delete ui;
}

void FormPx4Window::setPX4Thread(PX4Thread *px4)
{
    this->px4 = px4;
    planFlyProcess->setPx4(px4);
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
    px4->setTargetPosition(0, 0, 0, 0);
    px4->setArm(1);
}

void FormPx4Window::on_pb_take_off_clicked()
{
    px4->setTargetPosition(0, 0, 0.5, 0);
    updatePosText();
}

void FormPx4Window::on_pb_land_clicked()
{
    px4->setTargetPosition(0, 0, 0, 0);
    updatePosText();
}

void FormPx4Window::on_pb_set_position_clicked()
{
    float x = (float)ui->sb_x->value() / 1000.0;
    float y = (float)ui->sb_y->value() / 1000.0;
    float z = (float)ui->sb_z->value() / 1000.0;
    float yaw = (float)ui->sb_yaw->value() / 1000.0;

    px4->setTargetPosition(x, y, z, yaw);

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