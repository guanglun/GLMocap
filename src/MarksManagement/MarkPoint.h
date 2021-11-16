#ifndef MARKPOINT_H
#define MARKPOINT_H

#include "workspace.h"

class MarkPoint
{

private:

public:

    bool isMatch = false;
    int x=-1;
    int y=-1;

    MarkPoint();

    MarkPoint(int x,int y,bool isMatch = false)
    {
        this->x = x;
        this->y = y;
        this->isMatch = isMatch;
    }

};

#endif //POINT_H
