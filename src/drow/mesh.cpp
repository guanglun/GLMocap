#include "mesh.h"
#include <QtOpenGLExtensions/QOpenGLExtensions>
 
Mesh::Mesh(QOpenGLFunctions* glFunc, QOpenGLShaderProgram* shaderProgram, aiMatrix4x4 model)
    :   glFunc(glFunc)
    ,   shaderProgram(shaderProgram)
    ,   VBO(QOpenGLBuffer::VertexBuffer)
    ,   EBO(QOpenGLBuffer::IndexBuffer)
{
 
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            this->model(i,j)=model[i][j];
        }
    }
}
void Mesh::Draw()
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
 
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glFunc->glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
        // 获取纹理序号（diffuse_textureN 中的 N）
        QString number;
        QString name = textures[i]->type;
        if(name == "texture_diffuse")
          number = QString::number(diffuseNr++);
        else if(name == "texture_specular")
          number = QString::number(specularNr++);
        else if(name == "texture_normal")
          number = QString::number(normalNr++); // transfer unsigned int to stream
        else if(name == "texture_height")
          number = QString::number(heightNr++); // transfer unsigned int to stream
        textures[i]->texture.bind();
        shaderProgram->setUniformValue((name + number).toStdString().c_str(),i);
      }
    // 绘制网格
 
    QOpenGLVertexArrayObject::Binder bind(&VAO);
    shaderProgram->setUniformValue("model",model);
    glFunc->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
void Mesh::setupMesh()
{
 
    VAO.create();
    VAO.bind();
 
    VBO.create();
    EBO.create();
 
    VBO.bind();
    VBO.allocate(vertices.data(),vertices.size() * sizeof(Vertex));
 
    EBO.bind();
    EBO.allocate(indices.data(),indices.size() * sizeof(unsigned int));
 
 
    shaderProgram->enableAttributeArray(0);
    shaderProgram->setAttributeBuffer(0,GL_FLOAT,0,3,sizeof(Vertex));
 
    shaderProgram->enableAttributeArray(1);
    shaderProgram->setAttributeBuffer(1,GL_FLOAT,offsetof(Vertex,Normal),3,sizeof(Vertex));
 
    shaderProgram->enableAttributeArray(2);
    shaderProgram->setAttributeBuffer(2,GL_FLOAT,offsetof(Vertex,TexCoords),2,sizeof(Vertex));
 
    shaderProgram->enableAttributeArray(1);
    shaderProgram->setAttributeBuffer(3,GL_FLOAT,offsetof(Vertex, Tangent),3,sizeof(Vertex));
 
    shaderProgram->enableAttributeArray(1);
    shaderProgram->setAttributeBuffer(4,GL_FLOAT,offsetof(Vertex, Bitangent),3,sizeof(Vertex));
    VAO.release();
}