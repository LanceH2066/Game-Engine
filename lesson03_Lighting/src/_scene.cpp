#include "_scene.h"
#include<_lightSetting.h>

_lightSetting *myLight = new _lightSetting();

_scene::_scene()
{

}

_scene::~_scene()
{

}

GLint _scene::initGL()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    myLight->setLight(GL_LIGHT0);

    return true;
}

void _scene::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clear Bits
    glLoadIdentity();                                 // Identity Matrix

    glPushMatrix();
        glColor3f(1.0,0,0);
        glTranslatef(0.0,0.0,-8.0);
        glRotatef(30,0,1,0);
        glutSolidTeapot(1.5);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0,1,0);
        glTranslatef(8.0,0.0,-15.0);
        glRotatef(30,1,0,0);
        glutSolidCube(2.0);
    glPopMatrix();
}

void _scene::reSize(GLint width, GLint height)
{
    GLfloat aspectRatio = (GLfloat)width/(GLfloat)height;  // Resize Window Ratio
    glViewport(0,0,width,height);                           // Viewport
    glMatrixMode(GL_PROJECTION);                            // Perspective Projection Settings
    glLoadIdentity();                                       // Identity Matrix
    gluPerspective(45,aspectRatio,0.1,100.0);               // Projection
    glMatrixMode(GL_MODELVIEW);                             // Camera and Model Settings
    glLoadIdentity();                                       // Identity Matrix
}

