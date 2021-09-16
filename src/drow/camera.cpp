#include "camera.h"

Camera::Camera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ, QVector3D& worldUp) :
    m_position(posX, posY, posZ),
    m_target(targetX, targetY, targetZ),
    m_worldUp(worldUp)
{

}

Camera::Camera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ) :
    m_position(posX, posY, posZ),
    m_target(targetX, targetY, targetZ),
    m_worldUp(0.0f, 1.0f, 0.0f)
{

}

QVector3D Camera::position() const {
    return m_position;
}

QVector3D Camera::target() const {
    return m_target;
}

QMatrix4x4 Camera::view() const {
    QMatrix4x4 view;
    view.lookAt(m_position, m_target, m_worldUp);
    return view;
}

void Camera::setPositionX(float x) {
    m_position.setX(x);
}

void Camera::setPositionY(float y) {
    m_position.setY(y);
}

void Camera::setPositionZ(float z) {
    m_position.setZ(z);
}

void Camera::setPosition(float x, float y, float z) {
    m_position = QVector3D(x, y, z);
}

void Camera::setTargetX(float x) {
    m_target.setX(x);
}

void Camera::setTargetY(float y) {
    m_target.setY(y);
}

void Camera::setTargetZ(float z) {
    m_target.setZ(z);
}

void Camera::setTarget(float x, float y, float z) {
    m_target = QVector3D(x, y, z);
}

void Camera::setWorldUp(QVector3D& worldUp) {
    m_worldUp = worldUp;
}
