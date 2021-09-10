#ifndef PLANFLYPROCESS_H
#define PLANFLYPROCESS_H

#include "workspace.h"
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QWidget>
#include <QDateTime>
#include <QList>

#include "PlanPoint.h"
#include "PX4Thread.h"

class PlanFlyProcess:public QObject        
{
    Q_OBJECT
private:
    QList<PlanPoint *> ppList;
    PX4Thread *px4;
    bool isRuning = false;
    bool isStop = false;
public:
    
    PlanFlyProcess(QObject* parent = nullptr);
    QListView *lv_plan;
    QStandardItemModel *pModelPlan;
    void setQListView(QListView *lv_plan)
    {
        this->lv_plan = lv_plan;
    }
    void setModelPlan(QStandardItemModel *pModelPlan)
    {
        this->pModelPlan = pModelPlan;
    }
    void setPlan(QList<PlanPoint *> ppList);
    void setPx4(PX4Thread *px4);
    void stop(void);
public slots:
    void startSlot(void);
    
};
 
#endif // PLANFLYPROCESS_H