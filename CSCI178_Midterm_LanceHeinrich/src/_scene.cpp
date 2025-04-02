#include "_scene.h"
#define MAXBULLETS 100

_model *canonBase = new _model();
_player *player = new _player();
_inputs *inputs = new _inputs();
_bullets b[MAXBULLETS];
int bulletCount = 0;
float fireRate = 0.25f;
float dt = 0.0001;
_parallax *background = new _parallax();
_sounds *sounds = new _sounds();

_scene::_scene()
{
    //ctor
}

_scene::~_scene()
{
    //dtor
}
GLint _scene::initGL()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

    canonBase->initModel("images/cannon_base.png");
    player->initPlayer(3,1,"images/cannon.png");
    b[0].myTex->loadTexture("images/cannonballs.png");
    background->initPrlx("images/bg.png");

    for(int i = 0; i < MAXBULLETS; i++)
    {
        b[i].bInit(player);
    }

    sounds->initSounds();

    return true;
}

void _scene::reSize(GLint width, GLint height)
{
    GLfloat aspectRatio = (GLfloat)width/(GLfloat)height;
    // keep track of the resize window
    glViewport(0,0,width,height); // adjusting the viewport
    glMatrixMode(GL_PROJECTION);  // perspective projection settings
    glLoadIdentity();             // identity matrix
    gluPerspective(45,aspectRatio,0.1,100.0); // projection settings
    glMatrixMode(GL_MODELVIEW);   // camera and model settings
    glLoadIdentity();             // identity matrix

    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);
}

void _scene::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();
        glScalef(13.3,13.3,1);
        glDisable(GL_LIGHTING);
        background->drawBackground(dim.x,dim.y);
        glEnable(GL_LIGHTING);
    glPopMatrix();

    canonBase->drawModel();
    player->drawPlayer();
    player->playerActions();

    for(int i =0; i< MAXBULLETS;i++)
    {
        if (b[i].bLive) // Only active bullets
        {
            b[i].drawBullet(b[0].myTex->tex);
            b[i].bUpdate(player->plPos, b[i].bDes, sounds);
        }
    }

    fireRate+= dt;
}

int _scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_KEYDOWN:

        break;

    case WM_KEYUP: break;

    case WM_LBUTTONDOWN:
        for (int i = 0; i < MAXBULLETS; i++)
        {
            if (!b[i].bLive && fireRate >= 0.25f)
            {
                inputs->mouseEventDown(player, b[i], sounds);
                b[i].fireBullet();
                fireRate = 0;
                break; // Exit after firing one bullet
            }
        }
        break;
    case WM_RBUTTONDOWN:

        break;
    case WM_LBUTTONUP:
        inputs->mouseEventUp(player);
        break;
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:

        break;
    case WM_MOUSEMOVE:
        inputs->mouseMove(player, LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_MOUSEWHEEL:

        break;
    }
}
