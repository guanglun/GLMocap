#include "mesh.h"

Mesh::Mesh() :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{

}

Mesh::~Mesh() {
    m_vertexArray.destroy();
    m_indexBuffer.destroy();
    m_vertexBuffer.destroy();

    for(int i = 0; i < bones.size(); i++) {
        delete bones[i];
    }

    for(int i = 0; i < textures.size(); i++) {
        delete textures[i];
    }
}

void Mesh::setup(QOpenGLFunctions* gl)
{
    m_vertexArray.create();
    m_indexBuffer.create();
    m_vertexBuffer.create();

    m_vertexArray.bind();
    m_vertexBuffer.bind();
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

    m_indexBuffer.bind();
    m_indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer.allocate(indices.data(), indices.size() * sizeof(unsigned int));

    gl->glEnableVertexAttribArray(0);
    gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    gl->glEnableVertexAttribArray(1);
    gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

    gl->glEnableVertexAttribArray(2);
    gl->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

    m_vertexArray.release();
}

void Mesh::draw(QOpenGLFunctions* gl) {
    // Lighting is not implemented, so we handle at most one texture.
    if (textures.size() > 0) {
        textures[0]->bind();
    }

    m_vertexArray.bind();
    gl->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    m_vertexArray.release();
}
