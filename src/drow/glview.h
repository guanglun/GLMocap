#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

#include <gl/glu.h>



#include<QMouseEvent>
#include<QWheelEvent>



class GLView : public QOpenGLWidget ,
                     protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLView(QWidget *parent = 0);
    ~GLView();
    void setAngle(float rol, float pit, float yaw);
    
protected:
    //Three basic override function
    void initializeGL() Q_DECL_OVERRIDE;          //Initialize the OpenGL funciton and send data
    void paintGL() Q_DECL_OVERRIDE;               //Do opengl paint job
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;  //When the widget size changed it will work to

    
    
    void mousePressEvent(QMouseEvent*event);        //按下
    void mouseMoveEvent(QMouseEvent*event);         //按下移动
    void mouseReleaseEvent(QMouseEvent*event);      //松开
    void mouseDoubleClickEvent(QMouseEvent*event);  //双击
    void wheelEvent(QWheelEvent*event);             //滚轮
    
private:
    
    double eye[3];
    double center[3];
    double up[3];
    
    double yawCam, pitchCam, farCam;
    
    int clickX,clickY;
    float rol = 0,pit = 0,yaw = 0;
    
    void cameraTurn(double yaw, double pitch, double R_long);
    void cameraInit(double yaw, double pitch, double R_long);
    
    
private slots:
    void updateGL();
};
#endif // GLVIEW_H
