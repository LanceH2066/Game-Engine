#include "_scene.h"

_lightSetting *myLight = new _lightSetting();
_model *myModel = new _model();
_inputs *input = new _inputs();
_parallax *prlx1 = new _parallax();
_player *player = new _player();
_enemy enemies[20];

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);


    myModel->initModel("images/skin.jpg");
    prlx1->initParallax("images/forestBG.png", 0.005, false, true);

    player->initPlayer(10,1,"images/Sprites/IDLE.png","images/Sprites/RUN.png","images/Sprites/ATTACK.png");

    enemies[0].initEnemy("images/Sprites/mon.png");

    for(int i =0; i < 20; i++)
    {
        vec3 randPos = {(float)rand()/(float)(RAND_MAX)*5-2.5,-0.3,-2};
        enemies[i].position = randPos;
        enemies[i].speed = (float)((rand()%8)+1.0)/100.0;
    }

    return true;
}

void _scene::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clear Bits
    glLoadIdentity();                                 // Identity Matrix

    //myModel->drawModel();

    glPushMatrix();
        glDisable(GL_LIGHTING);
        glScalef(12.5,12.5,1);
        prlx1->drawBackground(dim.x,dim.y);
        glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
        glDisable(GL_LIGHTING);
        player->drawPlayer();
        player->playerActions();
        glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
        glDisable(GL_LIGHTING);
        for(int i =0; i < 20; i++)
        {
            enemies[i].drawEnemy(enemies[0].enemyTextureLoader->tex);
            enemies[i].enemyActions();
        }
        glEnable(GL_LIGHTING);
    glPopMatrix();
}

void _scene::reSize(GLint width, GLint height)
{
    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

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
            input->keyPressedPRLX(prlx1);

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
