#include "PlanFlyProcess.h"

PlanFlyProcess::PlanFlyProcess(QObject *parent)
{
}

void PlanFlyProcess::setPlan(QList<PlanPoint *> ppList)
{
    this->ppList = ppList;
}

void PlanFlyProcess::setPx4(PX4Thread *px4)
{
    this->px4 = px4;
}

void PlanFlyProcess::startSlot(void)
{
    int pc = 0;
    mlog->show("plan fly process start");
    if (isRuning)
    {
        mlog->show("error,running already");
        goto end;
    }
    isRuning = true;
    if (ppList.size() < 2)
    {
        mlog->show("error,plan size < 2");
        goto end;
    }

    for (pc = 0; pc < ppList.size(); pc++)
    { 
        ppList.at(pc)->setState(PLAN_POINT_STATE_WAIT);
    }

    for (pc = 0; pc < ppList.size(); pc++)
    {
        if (isStop)
        {
            goto end;
        }
        if (pc != 0)
        {
            ppList.at(pc - 1)->setState(PLAN_POINT_STATE_ARRIVE);
        }

        mlog->show("goto : " +
                   QString::number(ppList.at(pc)->mpd->x, 'f', 2) + "\t" +
                   QString::number(ppList.at(pc)->mpd->y, 'f', 2) + "\t" +
                   QString::number(ppList.at(pc)->mpd->z, 'f', 2));

        px4->setTargetPosition(ppList.at(pc)->mpd->x, ppList.at(pc)->mpd->y, ppList.at(pc)->mpd->z, 0);
        ppList.at(pc)->setState(PLAN_POINT_STATE_GOING);
        lv_plan->setCurrentIndex(pModelPlan->indexFromItem(ppList.at(pc)->item));

        for (int i = 0; i < ppList.at(pc)->mpd->waitTime / 10; i++)
        {
            if (isStop)
            {
                goto end;
            }
            QThread::msleep(10);
        }
    }
    ppList.at(pc - 1)->setState(PLAN_POINT_STATE_ARRIVE);
end:
    isRuning = false;
    isStop = false;
    mlog->show("plan fly process end");
}

void PlanFlyProcess::stop(void)
{
    mlog->show("stop slot");
    if (isRuning)
        isStop = true;
}