#ifndef GLDROW_H
#define GLDROW_H

#include <QOpenGLWidget>
#include "glview.h"

class GLDrow
{
public:
    GLDrow();
    static void DrowGrid(void);
    static void DrowCube(void);
    static void DrowCam(float size=0.1);
};

#endif // GLDROW_H
