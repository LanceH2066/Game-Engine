#include "_scene.h"

_lightSetting *myLight = new _lightSetting();
_model *myModel = new _model();
_inputs *input = new _inputs();
_model *teapot = new _model();

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

    myModel->initModel("images/skin.jpg");
    teapot->initModel("images/skin.jpg");
    return true;
}

void _scene::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clear Bits
    glLoadIdentity();                                 // Identity Matrix

    myModel->drawModel();
    teapot->drawModel();
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

int _scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_KEYDOWN:
            input->wParam = wParam;
            input->keyPressed(myModel);
            break;
        case WM_KEYUP:
            break;

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            input->wParam = wParam;
            input->mouseEventDown(myModel, LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            input->mouseEventUp();
            break;

        case WM_MOUSEMOVE:
            input->mouseMove(myModel, LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_MOUSEWHEEL:
            input->mouseWheel(myModel,(double)GET_WHEEL_DELTA_WPARAM(wParam));
            break;
    }
    return 0;
}
