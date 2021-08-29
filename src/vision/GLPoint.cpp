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

void GLPoint::show(void)
{
    mlog->show( QString::number(state) + " " +
                QString::number(x) + " " +
                QString::number(y) + " " +
                QString::number(id)
    );
}