#ifndef CAMVIEW_H
#define CAMVIEW_H

#include <QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QList>
//#include <QCheckBox>

#include "MultipleViewTriangulation.h"
#include "PlanPoint.h"

#ifdef __ENABLE_3D__
#include "model.h"
#endif

#include "camera.h"

class CamView : public QOpenGLWidget ,
                     protected QOpenGLFunctions
{
    Q_OBJECT

public:
#ifdef __ENABLE_3D__
    Model* m_model;
#endif    
    QMatrix4x4 m_modelMat;
    QMatrix4x4 m_projectionMat;
    Camera m_camera;
    QOpenGLShaderProgram* m_program;

    double eye[3];
    double center[3];
    double up[3];
    
    double yawCam, pitchCam, farCam;
    
    int clickX,clickY;
    float rol = 0,pit = 0,yaw = 0;
    float px = 0,py = 0,pz = 0;

    Vector3d *Xr;
    int size = 0;
    Vector3d *pos;
    bool isClearTrajectoryList = false;

    int view_fps_1s = 0;
    int point_fps_1s = 0;
    QList<PlanPoint *> ppList;
    QList<Vector3d> trajectoryList;

    enum Qt::CheckState cs_show_model = Qt::CheckState::Unchecked;
    enum Qt::CheckState cs_show_trajectory = Qt::CheckState::Unchecked;
    enum Qt::CheckState cs_show_plan = Qt::CheckState::Unchecked;
    enum Qt::CheckState cs_show_axis = Qt::CheckState::Unchecked;
    enum Qt::CheckState cs_show_cam = Qt::CheckState::Unchecked;

    CamView(QWidget *parent = 0);
    ~CamView();
    void setAngle(float rol, float pit, float yaw);
    void setPosition(Vector3d *Xr,int size,Vector3d *pos);
    void setPlan(QList<PlanPoint *> list);
    void loadModel(QString filename);
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
    


    void cameraTurn(double yaw, double pitch, double R_long);
    void cameraInit(double yaw, double pitch, double R_long);
    
private slots:
    void updateGL();
};
#endif // CAMVIEW_H
