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

void FormPx4Window::on_pb_load_plan_clicked()
{
    pModelPlan->clear();
    ppList.clear();

    ppList.append(new PlanPoint(0, 0, 0.5));
    ppList.append(new PlanPoint(-0.2, -0.2, 0.5));
    ppList.append(new PlanPoint(-0.2, 0.2, 0.5));
    ppList.append(new PlanPoint(0.2, 0.2, 0.5));
    ppList.append(new PlanPoint(0.2, -0.2, 0.5));
    ppList.append(new PlanPoint(-0.2, -0.2, 0.5));
    ppList.append(new PlanPoint(0, 0, 0.5));

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