#ifndef MESH_H
#define MESH_H

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>

struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector2D texCoords;
};

struct VertexWeight {
    unsigned int vertexIndex;
    float weight;
};

struct Bone {
    QString name;
    QMatrix4x4 offsetMatrix;
    QVector<VertexWeight> weights;
};

class Mesh {
public:
    Mesh();
    ~Mesh();
    void setup(QOpenGLFunctions* gl);
    void draw(QOpenGLFunctions* gl);

    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    QVector<Bone*> bones;
    QVector<QOpenGLTexture*> textures;

private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QOpenGLVertexArrayObject m_vertexArray;
};

#endif // MESH_H
