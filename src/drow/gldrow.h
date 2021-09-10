#ifndef GLDROW_H
#define GLDROW_H

#include <QOpenGLWidget>
#include <QGLWidget>
#include "glview.h"
#include <QtMath>

class GLDrow
{
public:
    GLDrow();
    static void DrowGrid(void);
    static void DrowCube(void);
    static void DrowCam(float size = 0.1);
    // 绘制立方体
    static void drawCube();
    // 绘制球体
    static void drawSphere(GLfloat xx = 0.0,
                           GLfloat yy = 0.0,
                           GLfloat zz = 0.0,
                           GLfloat radius = 0.02,
                           GLfloat M = 100.0,
                           GLfloat N = 100.0);

    static void DrowArrow(float sx = 0.0,
                           float sy = 0.0,
                           float sz = 0.0,
                           float tx = 0.1,
                           float ty = 0.1,
                           float tz = 0.1,
                           float radius = 0.004);
};

#endif // GLDROW_H
