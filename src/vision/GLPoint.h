#ifndef GLPOINT_H
#define GLPOINT_H

#include "workspace.h"


typedef enum POINT_STATE{
    POINT_STATE_NEW_FOUND,
    POINT_STATE_FOUND_MARK
}POINT_STATE;

#define FILTER_SIZE 4
class GLPoint
{

private:

public:
    POINT_STATE state = POINT_STATE_NEW_FOUND;
    int imageIndex = -1;
    int x=-1;
    int y=-1;
    int id = -1;
    
    int filter[FILTER_SIZE][2];
    int filterCount = 0;
    bool isFilerInit = false;

    GLPoint();
    GLPoint(POINT_STATE state,
    int imageIndex,
    int x,
    int y,
    int id);
    
    GLPoint(POINT_STATE state,int imageIndex,int x,int y)
    {
        this->state = state;
        this->imageIndex = imageIndex;
        this->x = x;
        this->y = y;
    }

    void initFiler(void);
    void setXY(int x,int y);
    void show(void);
};

#endif //POINT_H
