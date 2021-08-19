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
    DrowCube();

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

// 绘制球体
// 球心坐标为（x，y，z），球的半径为radius，M，N分别表示球体的横纵向被分成多少份
void GLDrow::drawSphere(GLfloat xx, GLfloat yy, GLfloat zz,
                          GLfloat radius, GLfloat M, GLfloat N)
{

    float step_z = M_PI / M;
    float step_xy = 2 * M_PI / N;
    float x[4], y[4], z[4];

    float angle_z = 0.0;
    float angle_xy = 0.0;
    int i = 0, j = 0;
    glBegin(GL_QUADS);
    for(i = 0; i < M; i++)
    {
        angle_z = i * step_z;

        for(j = 0; j < N; j++)
        {
            angle_xy = j * step_xy;

            x[0] = radius * sin(angle_z) * cos(angle_xy);
            y[0] = radius * sin(angle_z) * sin(angle_xy);
            z[0] = radius * cos(angle_z);

            x[1] = radius * sin(angle_z + step_z) * cos(angle_xy);
            y[1] = radius * sin(angle_z + step_z) * sin(angle_xy);
            z[1] = radius * cos(angle_z + step_z);

            x[2] = radius * sin(angle_z + step_z) * cos(angle_xy + step_xy);
            y[2] = radius * sin(angle_z + step_z) * sin(angle_xy + step_xy);
            z[2] = radius * cos(angle_z + step_z);

            x[3] = radius * sin(angle_z) * cos(angle_xy + step_xy);
            y[3] = radius * sin(angle_z) * sin(angle_xy + step_xy);
            z[3] = radius * cos(angle_z);
            for(int k = 0; k < 4; k++)
            {
                glColor3f(sin(angle_z), cos(angle_z), tan(angle_z));
                //glTexCoord2f(0.1f, 0.1f);
                glVertex3f(xx + x[k], yy + y[k], zz + z[k]);
            }
        }
    }
    glEnd();
}