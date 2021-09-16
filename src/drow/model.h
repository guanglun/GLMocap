#ifndef MODEL_H
#define MODEL_H

#include <QDir>
#include <QVector>
#include <QString>
#include <QFileInfo>
#include <QMatrix4x4>
#include <QOpenGLFunctions>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "mesh.h"

class Model
{
public:
    struct Node
    {
        ~Node()
        {
            for (int i = 0; i < children.size(); i++)
            {
                delete children[i];
            }
        }
        QString name;
        QVector<Node *> children;
        QMatrix4x4 transformation;
        QVector<unsigned int> meshes;
    };

    Model(QString filename, QOpenGLFunctions *context);
    ~Model();
    void draw(QOpenGLFunctions *context);

private:
    void processNode(QOpenGLFunctions *context, aiNode *source, Node *node, const aiScene *scene);
    void processMesh(aiMesh *source, Mesh *mesh, const aiScene *scene);
    void loadTextures(Mesh *mesh, aiMaterial *material, aiTextureType type);

    Node *m_rootNode;
    QFileInfo m_fileInfo;
    QVector<Mesh *> m_meshes;
};

#endif // MODEL_H
