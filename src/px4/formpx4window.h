#ifndef FORMPX4WINDOW_H
#define FORMPX4WINDOW_H

#include <QWidget>
#include <QDateTime>
#include <QCloseEvent> 
#include <QFileInfo>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QThread>

#include "PlanPoint.h"

#include "workspace.h"
#include "PX4Thread.h"
#include "PlanFlyProcess.h"

namespace Ui {
class FormPx4Window;
}

class FormPx4Window : public QWidget
{
    Q_OBJECT
    PX4Thread *px4;
private:
    QStandardItemModel *pModelPlan;
    QStandardItem *pItem;
    QThread planFlyProcessThread;
    PlanFlyProcess *planFlyProcess;
    QList<PlanPoint *> ppList;
    void updatePosText();
public:
    explicit FormPx4Window(QWidget *parent = 0);
    ~FormPx4Window();
    void setPX4Thread(PX4Thread *px4);
    void setItem(PlanPoint *pp);
private slots:
    void on_pb_ini_mavlink_clicked();
    void on_pb_disarm_clicked();
    void on_pb_arm_clicked();
    void on_pb_take_off_clicked();
    void on_pb_land_clicked();
    void on_pb_set_position_clicked();
    void on_pb_load_plan1_clicked();
    void on_pb_load_plan2_clicked();
    void on_pb_load_plan3_clicked();
    void on_pb_start_plan_clicked();
    void on_pb_stop_plan_clicked();
signals:
    void onPlanSignals(QList<PlanPoint *>list);
    void startPlanSignal(void);
    void stopPlanSignal(void);
private:
    Ui::FormPx4Window *ui;
    
protected: 

};

#endif // 
