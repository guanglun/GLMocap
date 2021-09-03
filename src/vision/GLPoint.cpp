#include "GLPoint.h"

GLPoint::GLPoint()
{
}

GLPoint::GLPoint(POINT_STATE state,
                 int imageIndex,
                 int x,
                 int y,
                 int id)
{
    this->state = state;
    this->x = x;
    this->y = y;
    this->id = id;
}

void GLPoint::initFiler(void)
{
    isFilerInit = false;
}

void GLPoint::setXY(int x, int y)
{
    int sumx = 0, sumy = 0;

    if (isFilerInit == false)
    {
        isFilerInit = true;
        filterCount = 0;

        for (int i = 0; i < FILTER_SIZE; i++)
        {
            filter[i][0] = x;
            filter[i][1] = y;
        }

        this->x = x;
        this->y = y;
    }
    else
    {

        filter[filterCount][0] = x;
        filter[filterCount][1] = y;

        for (int i = 0; i < FILTER_SIZE; i++)
        {
            sumx += filter[i][0];
            sumy += filter[i][1];
        }
        this->x = sumx / FILTER_SIZE;
        this->y = sumy / FILTER_SIZE;

        filterCount++;

        if (filterCount >= FILTER_SIZE)
        {
            filterCount = 0;
        }
    }
}

void GLPoint::show(void)
{
    mlog->show(QString::number(state) + " " +
               QString::number(x) + " " +
               QString::number(y) + " " +
               QString::number(id));
}