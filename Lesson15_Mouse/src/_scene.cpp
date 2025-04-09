#include "_scene.h"
#include<_lightsetting.h>
#include<_model.h>
#include<_inputs.h>
#include<_parallax.h>
#include<_player.h>
#include<_enms.h>
#include<_collisionckeck.h>
#include<_sounds.h>
#include<_bullets.h>
#include<_particles.h>

#define MAXBULLETS 20
#define ENMCOUNT 20


_lightSetting *myLight = new _lightSetting();
_model *myModel = new _model();    // creating instance for model
_inputs *input = new _inputs();
_parallax *prlx = new _parallax();
_player *ply = new _player();
_enms enm[ENMCOUNT]; // = new _enms();
_bullets b[MAXBULLETS];
_collisionCkeck *hit = new _collisionCkeck();
_sounds *snds = new _sounds();
int bulletCount = 0;
_particles *p= new _particles();

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

   glEnable(GL_BLEND); // take off the transparent background
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

  // glEnable(GL_COLOR_MATERIAL);
   myLight->setLight(GL_LIGHT0);

   dim.x = GetSystemMetrics(SM_CXSCREEN);
   dim.y = GetSystemMetrics(SM_CYSCREEN);

   myModel->initModel("images/skin.jpg");
   prlx->initPrlx("images/prlx.jpg");
   ply->initPlayer(6,4,"images/player.png");
   enm[0].initEnms("images/mon.png");
   b[0].myTex->loadTexture("images/arr.png");
   snds->initSounds();
   snds->playMusic("sounds/MF-W-90.XM");


   for(int i=0;i<ENMCOUNT;i++)
   {
       enm[i].pos.x = (float)rand()/(float)(RAND_MAX)*5-2.5;
       enm[i].pos.y = -0.65;

       enm[i].scale.x = enm[i].scale.y = (float)(rand()%12)/50.0;
       enm[i].speed = (float)((rand()%8)+1.0)/100.0;

       b[i].bInit(ply->plPos);
   }

   p->initParticles(ply->plPos,"images/part.png");

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

    gluLookAt(0,0,1, 0,0,0,0,1,0);

    //myModel->drawModel();

    glPushMatrix();
    glScalef(13.3,13.3,1);
    glDisable(GL_LIGHTING);
    prlx->drawBackground(dim.x,dim.y);
  //  prlx->scroll(true,"left",0.005);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glDisable(GL_LIGHTING);
    ply->drawPlayer();
    ply->playerActions();

    for(int i =0; i< 20;i++){

   if(hit->isRadialCol(enm[i].pos, ply->plPos,0.2,0.3, 0.0002))
    {
        if(enm[i].actionTrigger == enm[i].RIGHTWALK)
        enm[i].actionTrigger = enm[i].ROTATELEFT;
        else if ((enm[i].actionTrigger == enm[i].LEFTWALK))
        enm[i].actionTrigger = enm[i].ROTATERIGHT;
    }

    enm[i].drawEnms(enm[0].myTex->tex);
    enm[i].actions();

    b[i].drawBullet(b[0].myTex->tex);
    b[i].bUpdate(ply->plPos,b[i].bDes);

    glPushMatrix();
    p->generateParticles(mouseX,mouseY);
    p->drawParticles();
    p->updateParticles();
    glPopMatrix();
 }

glEnable(GL_LIGHTING);
}

void _scene::mouseMapping(int x,int y)
{
    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

    GLint viewPort[4];
    GLdouble modelViewM[16];
    GLdouble projectionM[16];
    GLfloat winX, winY, winZ;

    glGetDoublev(GL_MODELVIEW_MATRIX,modelViewM);
    glGetDoublev(GL_PROJECTION_MATRIX,projectionM);
    glGetIntegerv(GL_VIEWPORT, viewPort);

    winX = (GLfloat)x;
    winY = (GLfloat)(viewPort[3]-y);
    glReadPixels(x,int(winY),1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);    // for 3d

    //winZ = -4.0;    // for 2d unless you have depth in your game

    float scale = 60*(dim.x/dim.y);

    mouseX = (GLfloat)(x-dim.x/2.0)/scale;
    mouseY = (GLfloat)(y-dim.y/2.0)/scale;
    mouseZ = -100;

    gluUnProject(winX,winY,winZ,modelViewM,projectionM,viewPort,&mouseX,&mouseY,&mouseZ);



}

int _scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_KEYDOWN:
           input->wParam = wParam;
           input->keyPressed(myModel);
           input->keyPressedPRLX(prlx);
           input->keyPressed(snds,"sounds/explosion.wav");
            bulletCount++;
            bulletCount= bulletCount%MAXBULLETS;
           input->keyPressed(b[bulletCount]);
        break;


    case WM_KEYUP: break;

    case WM_LBUTTONDOWN:
        mouseMapping(LOWORD(lParam),HIWORD(lParam));
        cout << mouseX << "       " << mouseY << endl;
        break;
    case WM_RBUTTONDOWN:
        input->wParam = wParam;
        input->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));
        break;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        input->mouseEventUp();
        break;
    case WM_MOUSEMOVE:
        input->mouseMove(myModel,LOWORD(lParam),HIWORD(lParam));
        break;
    case WM_MOUSEWHEEL:
        input->mouseWheel(myModel,(double)GET_WHEEL_DELTA_WPARAM(wParam));
        break;
    }
}
