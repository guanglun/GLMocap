#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

class Camera {
public:
    Camera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ, QVector3D& worldUp);
    Camera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);

    QVector3D position() const;
    QVector3D target() const;
    QMatrix4x4 view() const;

    void setPositionX(float x);
    void setPositionY(float y);
    void setPositionZ(float z);
    void setPosition(float x, float y, float z);

    void setTargetX(float x);
    void setTargetY(float y);
    void setTargetZ(float z);
    void setTarget(float x, float y, float z);

    void setWorldUp(QVector3D& worldUp);

private:
    QVector3D m_position;
    QVector3D m_target;
    QVector3D m_worldUp;
};

#endif // CAMERA_H
