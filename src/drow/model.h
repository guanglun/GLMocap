#ifndef MODEL_H
#define MODEL_H
 
#include "mesh.h"
#include <QDir>
 
class Model
{
public:
    void draw();
    void destroy();
    static Model* createModel(QString path,QOpenGLContext* context,QOpenGLShaderProgram* shaderProgram);
 
private:
    Model(QString path,QOpenGLContext* context,QOpenGLShaderProgram* shaderProgram);
    ~Model();
    QOpenGLContext* context;          //opengl函数入口
    QOpenGLShaderProgram* shaderProgram ;   //着色器程序
 
    /*  模型数据  */
    QVector<Texture*>textures_loaded;       //纹理
    QVector<Mesh*> meshes;                  //网格
    QDir directory;                         //模型所在路径
 
    //递归遍历结点
    void processNode(aiNode *node, const aiScene *scene,aiMatrix4x4 mat4=aiMatrix4x4());
 
    //加载网格
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene, aiMatrix4x4 model);
 
    //加载材质纹理
    QVector<Texture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type,QString typeName);
 
};
#endif // MODEL_H