#include <iostream>
#include "GL/glut.h"

using namespace std;

int xx = 0, yy = 0;

void RenderScene()
{
    //清空颜色缓冲区，填充的颜色由 glClearColor( 0, 0.0, 0.0, 1 ); 指定为黑色

    glClear( GL_COLOR_BUFFER_BIT );
    {
        glPointSize( 9 );//指定点的大小，9个像素单位
        glColor3f( 1.0f, 0.0f, 0.0f );//指定点的颜色
        glBegin( GL_POINTS );//开始画点
        {
            glVertex3f(0.0f, 0.0f, 0.0f); // 在坐标为(0,0,0)的地方绘制了一个点
        }
        glEnd();//结束画点
    }

    {
        glColor3f( 0.0f, 0.0f, 1.0f );//指定线的颜色,蓝色

        glBegin( GL_LINES );
        {
            // x-axis
            for(int i =0;i<100;i++){
                glVertex3f( -1.0f*i, -1.0f*i, 0.0f);
                glVertex3f( 1.0f*i, 1.0f*i, 0.0f);
            }

            glVertex3f( -100.0f, 0.0f, 0.0f);
            glVertex3f( 100.0f, 0.0f, 0.0f);

    

            glVertex3f( 0.0f, -100.0f, 0.0f);
            glVertex3f( 0.0f, 100.0f, 0.0f);

        }
        glEnd();

    }

    glutSwapBuffers();
}

void Display(){
    glClear( GL_COLOR_BUFFER_BIT );
    {
        glColor3f( 1.0f, 0.0f, 0.0f );//指定点的颜色,红色

        glPointSize( 5 );//指定点的大小，9个像素单位

        glBegin( GL_POINTS );//开始画点

        {
            glVertex3f(1.0f*xx,1.0f*yy, 0.0f); // 在坐标为(0,0,0)的地方绘制了一个点

        }
        glEnd();//结束画点

    }

glutSwapBuffers();

}

void SetupRC()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 1 );//以RGB(0,0,0)即黑色来清空颜色缓冲区

    glColor3f( 1.0f, 0.0f, 0.0f );//以RGB(1,0,0)即红色来绘制图形

}


void ChangeSize( GLsizei w, GLsizei h )
{
    GLfloat nRange = 200.0f;

    // Prevent a divide by zero

    if(h == 0)
        h = 1;

    // Set Viewport to window dimensions

    glViewport(0, 0, w, h);

    // Reset projection matrix stack

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Establish clipping volume (left, right, bottom, top, near, far)

    if (w <= h) 
        glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    else 
        glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

    // Reset Model view matrix stack

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SpecialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)
        yy += 5.0f;

    if(key == GLUT_KEY_DOWN)
        yy -= 5.0f;

    if(key == GLUT_KEY_LEFT)
        xx -= 5.0f;

    if(key == GLUT_KEY_RIGHT)
        xx += 5.0f;



    glutPostRedisplay();// this will refresh the window, so, it works the same to call RenderScene() directly 

}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (200, 200);
    glutInitWindowPosition (10, 10);
    glutCreateWindow( "Point examples" );
    glutDisplayFunc( Display );
     glutReshapeFunc( ChangeSize );
    glutSpecialFunc( SpecialKeys );
    SetupRC();
    glutMainLoop();

    
    return 1;
}