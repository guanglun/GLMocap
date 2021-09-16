#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QWheelEvent>
#include <QMouseEvent>

#include "model.h"
#include "camera.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    GLWidget(QWidget* parent);
    ~GLWidget();

    void loadModel(QString filename);

public slots:
    void setPolygonMode(bool wireframe);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void wheelEvent(QWheelEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    Model* m_model;
    QMatrix4x4 m_modelMat;
    QMatrix4x4 m_projectionMat;

    Camera m_camera;
    QTimer* m_timer;
    QPoint m_lastMousePos;

    QOpenGLShaderProgram* m_program;
};

#endif // GLWIDGET_H
