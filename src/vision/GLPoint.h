#ifndef GLPOINT_H
#define GLPOINT_H

#include "workspace.h"


typedef enum POINT_STATE{
    POINT_STATE_NEW_FOUND,
    POINT_STATE_FOUND_MARK
}POINT_STATE;

class GLPoint
{

private:

public:
    POINT_STATE state = POINT_STATE_NEW_FOUND;
    int imageIndex = -1;
    int x=-1;
    int y=-1;
    int id = -1;
    
    GLPoint();
    GLPoint(POINT_STATE state,int imageIndex,int x,int y)
    {
        this->state = state;
        this->imageIndex = imageIndex;
        this->x = x;
        this->y = y;
    }
};

#endif //POINT_H
