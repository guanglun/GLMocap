#include "glview.h"
#include "gldrow.h"
#include "workspace.h"
#include <QtMath>

#include <Eigen/Geometry>
#include <Eigen/Core>
#include <Eigen/Dense> 


using namespace Eigen;

GLView::GLView(QWidget *parent)
{

}

GLView::~GLView()
{

}


void GLView::initializeGL()
{
    initializeOpenGLFunctions();
    
    glEnable(GL_TEXTURE_2D);
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    

    cameraInit(10,30,4.0f);
}

void GLView::updateGL()
{

}

void GLView::paintGL()
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (eye[0],      eye[1],     eye[2],
               center[0],   center[1],  center[2], 
               up[0],       up[1],      up[2]);  
    

    glPushMatrix();

    
    //欧拉角角度不一致，需要转换一下
    
    glRotatef(-rol, 1, 0, 0);
    glRotatef(pit, 0, 1, 0);
    glRotatef(yaw, 0, 0, 1);
    
    GLDrow::DrowCube();
    
    glPopMatrix();
    
    
    glPushMatrix();
    glTranslatef(0.0,0.0,-0.8);//下移
    GLDrow::DrowGrid();
    glPopMatrix();
    
    
    QMetaObject::invokeMethod(this,"update",Qt::QueuedConnection); 

    
}

void GLView::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 0.1, 100.0);
    
    cameraTurn(yawCam,pitchCam,farCam);
    
    
    //DBG("resizeGL");
    
    
}

void GLView::setAngle(float rol, float pit, float yaw)
{
    this->rol = rol;
    this->pit = pit;
    this->yaw = yaw;
}

void GLView::cameraInit(double yaw, double pitch, double R_long)
{
    
    yawCam      = yaw;
    pitchCam    = pitch;
    farCam      = R_long;
}

void GLView::cameraTurn(double yaw, double pitch, double R_long)
{
    
    if (pitch > 90)
        pitch = 90;
    else if (pitch < -90)
        pitch = -90;
    
    double angle_yaw    = yaw * M_PI / 180;
    double angle_pitch  = pitch * M_PI / 180;

    Vector3d v(0,1,0);           //{ { 0 }, { 1 }, { 0 } };
    Vector3d v_top(0,0,1);       // { { 0 }, { 0 }, { 1 } };
    Vector3d yaw_k(0,0,1);       // { { 0 }, { 0 }, { 1 } };
    Vector3d pitch_k(1,0,0);     // { { 1 }, { 0 }, { 0 } };

    Matrix3d E;
    E << 1,0,0,
         0,1,0,
         0,0,1;

    
    Matrix3d G_yaw;
    G_yaw <<    0,          -yaw_k[2],  yaw_k[1],
                yaw_k[2],   0,          -yaw_k[0],
                -yaw_k[1],  yaw_k[0],   0;


    v = E * qCos(angle_yaw) * v + (1 - qCos(angle_yaw)) * yaw_k * yaw_k.transpose() * v + qSin(angle_yaw) * G_yaw * v;
    pitch_k = E * qCos(angle_yaw) * pitch_k + (1 - qCos(angle_yaw)) * yaw_k * yaw_k.transpose() * pitch_k + qSin(angle_yaw) * G_yaw * pitch_k;

   
    Matrix3d G_pitch;
    G_pitch <<  0,          -pitch_k[2],    pitch_k[1],
                pitch_k[2], 0,              -pitch_k[0],
                -pitch_k[1],pitch_k[0],     0;
    
    v = E * qCos(angle_pitch) * v + (1 - qCos(angle_pitch)) * pitch_k * pitch_k.transpose() * v + qSin(angle_pitch) * G_pitch * v;
    v = v * R_long;

    pitch_k <<  1,0,0;
             
    v_top = E * qCos(angle_yaw) * v_top + (1 - qCos(angle_yaw)) * yaw_k * yaw_k.transpose() * v_top + qSin(angle_yaw) * G_yaw * v_top;
    pitch_k = E * qCos(angle_yaw) * pitch_k + (1 - qCos(angle_yaw)) * yaw_k * yaw_k.transpose() * pitch_k + qSin(angle_yaw) * G_yaw * pitch_k;

    G_pitch <<  0,          -pitch_k[2],    pitch_k[1],
                pitch_k[2], 0,              -pitch_k[0],
                -pitch_k[1],pitch_k[0],     0;

    v_top = E * qCos(angle_pitch) * v_top + (1 - qCos(angle_pitch)) * pitch_k * pitch_k.transpose() * v_top + qSin(angle_pitch) * G_pitch * v_top;
    
    eye[0] = v[0];
    eye[1] = v[1];
    eye[2] = v[2];
    center[0] = 0;
    center[1] = 0;
    center[2] = 0;
    up[0] = v_top[0];
    up[1] = v_top[1];
    up[2] = v_top[2];
    
    //DBG("yawCam:%0.2f\tpitchCam:%0.2f\tfarCam:%0.2f\t",yaw,pitch,R_long);
}

void GLView::mousePressEvent(QMouseEvent*event){
    if(event->buttons()==Qt::LeftButton){               //如果鼠标按下的是左键
        clickX = event->globalX();
        clickY = event->globalY();
        //DBG("LeftButton %d %d",event->globalX(),event->globalY());
    }else if(event->buttons()==Qt::RightButton){         //如果鼠标按下的是右键
        //DBG("RightButton %d %d",event->globalX(),event->globalY());
    }
}

void GLView::mouseMoveEvent(QMouseEvent*event){
    if(event->buttons()==Qt::LeftButton){//如果鼠标按下的是左键
        //DBG("mouseMoveEvent %d %d",event->globalX(),event->globalY());
        
        if(clickX != event->globalX())
        {
            yawCam += (clickX-event->globalX()) * 1.0;
            clickX = event->globalX();
        }
        
        if(clickY != event->globalY())
        {
            pitchCam -= (clickY-event->globalY()) * 1.0;
            clickY = event->globalY();
        }
        
        cameraTurn(yawCam,pitchCam,farCam);

    }
}
 
void GLView::mouseReleaseEvent(QMouseEvent*event){

}
 
void GLView::mouseDoubleClickEvent(QMouseEvent*event){
}

void GLView::wheelEvent(QWheelEvent*event){
    
    DBG("wheelEvent %d",event->delta());
    
    if(event->delta() > 0)
    {
        farCam -= 0.1;
    }
    else{
        farCam += 0.1;
    }

    cameraTurn(yawCam,pitchCam,farCam);
    
    
}

