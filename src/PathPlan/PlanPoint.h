#ifndef PLANPOINT_H
#define PLANPOINT_H

#include <QStandardItem>
#include "workspace.h"
#include "MuItemPoint.h"

typedef enum PLAN_POINT_STATE
{
    PLAN_POINT_STATE_WAIT,
    PLAN_POINT_STATE_GOING,
    PLAN_POINT_STATE_ARRIVE,
} PLAN_POINT_STATE;

typedef enum PLAN_POINT_TYPE
{
    PLAN_POINT_TYPE_WT,
    PLAN_POINT_TYPE_CP,
    PLAN_POINT_TYPE_WTCP,
} PLAN_POINT_TYPE;

class PlanPoint : public QObject
{
    Q_OBJECT

private:
public:
    PLAN_POINT_STATE state = PLAN_POINT_STATE_WAIT;
    PLAN_POINT_TYPE type = PLAN_POINT_TYPE_WT;

    MuItemPointData *mpd;
    QStandardItem *item;
    PlanPoint();

    PlanPoint(float x, float y, float z, int waitTime = 1000, PLAN_POINT_TYPE type = PLAN_POINT_TYPE_WT)
    {
        mpd = new MuItemPointData();
        item = new QStandardItem();

        setState(PLAN_POINT_STATE_WAIT, false);

        this->type = type;
        if (type == PLAN_POINT_TYPE_WT)
        {
            mpd->typeString = "WT";
        }
        else if (type == PLAN_POINT_TYPE_CP)
        {
            mpd->typeString = "CP";
        }
        else if (type == PLAN_POINT_TYPE_WTCP)
        {
            mpd->typeString = "WTCP";
        }

        mpd->x = x;
        mpd->y = y;
        mpd->z = z;

        mpd->waitTime = waitTime;

        item->setData(QVariant::fromValue(*mpd), Qt::UserRole + 1);

        connect(this, SIGNAL(setStateSignal(PLAN_POINT_STATE, bool)), this, SLOT(setStateSlot(PLAN_POINT_STATE, bool)));
    }

    void setState(PLAN_POINT_STATE state, bool isFlash = true)
    {
        emit setStateSignal(state,isFlash);
    }

    void flashList(void)
    {
        item->setData(QVariant::fromValue(*mpd));
    }
private slots:
    void setStateSlot(PLAN_POINT_STATE state,bool isFlash)
    {
        this->state = state;
        if (state == PLAN_POINT_STATE_WAIT)
        {
            mpd->stateString = "wait";
        }
        else if (state == PLAN_POINT_STATE_GOING)
        {
            mpd->stateString = "going";
        }
        else if (state == PLAN_POINT_STATE_ARRIVE)
        {
            mpd->stateString = "arrive";
        }

        if (isFlash)
        {
            flashList();
        }
    }
signals:
    void setStateSignal(PLAN_POINT_STATE state,bool isFlash);
};

#endif //POINT_H
