#include "gldrow.h"

GLDrow::GLDrow()
{
    
}

//画网格
void GLDrow::DrowGrid(void)
{
    glLineWidth(1);//设置线段宽度
    glColor3f(1.0, 1.0, 1.0);
    for (double cnt=-4.0;cnt<4.2;cnt+=0.2)
    {
        
        glBegin(GL_LINES);
        glVertex3f(-4.0, cnt, -0.5);
        glVertex3f(4.0, cnt, -0.5);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(cnt, -4.0,  -0.5);
        glVertex3f(cnt, 4.0,  -0.5);
        glEnd();
    }

    glLineWidth(3);//设置线段宽度
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(-4.0, -4.0, -0.5);
    glVertex3f(-4.0, -4.0, 0.5);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(-4.0, -4.0, -0.5);
    glVertex3f(-4.0, -3.0, -0.5);
    glEnd();

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(-4.0, -4.0, -0.5);
    glVertex3f(-3.0, -4.0, -0.5);
    glEnd();    
}

//画立方体
void GLDrow::DrowCube(void)
{
    //画立方体的6个面  
    glBegin(GL_QUADS);  
    
    glColor3f(1.0, 0.0, 0.0);
    glNormal3f( 0.0F, 0.0F, 1.0F);  
    glVertex3f( 0.5f, 0.5f, 0.5f);  
    glVertex3f(-0.5f, 0.5f, 0.5f);  
    glVertex3f(-0.5f,-0.5f, 0.5f);  
    glVertex3f( 0.5f,-0.5f, 0.5f);  
    //1----------------------------  
    
    glColor3f(0.0, 1.0, 0.0);
    glNormal3f( 0.0F, 0.0F,-1.0F);  
    glVertex3f(-0.5f,-0.5f,-0.5f);  
    glVertex3f(-0.5f, 0.5f,-0.5f);  
    glVertex3f( 0.5f, 0.5f,-0.5f);  
    glVertex3f( 0.5f,-0.5f,-0.5f);  
    //2---------------------------- 
    
    glColor3f(0.0, 0.0, 1.0);
    glNormal3f( 0.0F, 1.0F, 0.0F);  
    glVertex3f( 0.5f, 0.5f, 0.5f);  
    glVertex3f( 0.5f, 0.5f,-0.5f);  
    glVertex3f(-0.5f, 0.5f,-0.5f);  
    glVertex3f(-0.5f, 0.5f, 0.5f);  
    //3----------------------------  
    
    glColor3f(1.0, 1.0, 0.0);
    glNormal3f( 0.0F,-1.0F, 0.0F);  
    glVertex3f(-0.5f,-0.5f,-0.5f);  
    glVertex3f( 0.5f,-0.5f,-0.5f);  
    glVertex3f( 0.5f,-0.5f, 0.5f);  
    glVertex3f(-0.5f,-0.5f, 0.5f);  
    //4---------------------------- 
    
    glColor3f(0.0, 1.0, 1.0);
    glNormal3f( 1.0F, 0.0F, 0.0F);  
    glVertex3f( 0.5f, 0.5f, 0.5f);  
    glVertex3f( 0.5f,-0.5f, 0.5f);  
    glVertex3f( 0.5f,-0.5f,-0.5f);  
    glVertex3f( 0.5f, 0.5f,-0.5f);  
    //5----------------------------  
    
    glColor3f(1.0, 0.0, 1.0);
    glNormal3f(-1.0F, 0.0F, 0.0F);  
    glVertex3f(-0.5f,-0.5f,-0.5f);  
    glVertex3f(-0.5f,-0.5f, 0.5f);  
    glVertex3f(-0.5f, 0.5f, 0.5f);  
    glVertex3f(-0.5f, 0.5f,-0.5f);  
    //6----------------------------*/  
    glEnd();
}

void GLDrow::DrowCam(float size)
{
    // glRotatef(90, 1, 0, 0);
    // glRotatef(180, 0, 0, 1);
    // glRotatef(-90, 0, 1, 0);

    glColor3f(1.0, 0.0, 0.0);
    glutWireCube(size);

    glBegin(GL_LINES);
    glVertex3f(size/2, size/2, size/2);
    glVertex3f(size/2*2, size/2*2, size/2*3);

    glVertex3f(-size/2, size/2, size/2);
    glVertex3f(-size/2*2, size/2*2, size/2*3);

    glVertex3f(size/2, -size/2, size/2);
    glVertex3f(size/2*2, -size/2*2, size/2*3);

    glVertex3f(-size/2, -size/2, size/2);
    glVertex3f(-size/2*2, -size/2*2, size/2*3);

    glVertex3f(size/2*2, size/2*2, size/2*3);
    glVertex3f(-size/2*2, size/2*2, size/2*3);

    glVertex3f(size/2*2, size/2*2, size/2*3);
    glVertex3f(size/2*2, -size/2*2, size/2*3);

    glVertex3f(-size/2*2, size/2*2, size/2*3);
    glVertex3f(-size/2*2, -size/2*2, size/2*3);

    glVertex3f(size/2*2, -size/2*2, size/2*3);
    glVertex3f(-size/2*2, -size/2*2, size/2*3);

    glEnd();

    //glRotatef(-90, 0, 1, 0);

    //方向
    glLineWidth(3);//设置线段宽度
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);
    glEnd();

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);
    glEnd();    
}