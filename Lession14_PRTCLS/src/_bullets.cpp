#include "_bullets.h"

_bullets::_bullets()
{
    //ctor
}

_bullets::~_bullets()
{
    //dtor
}
void _bullets::bInit(vec3 pPos)
{
    bPos= pPos;

    bDes.x =5;    // use your own destination
    bDes.y = 0;
    bDes.z = pPos.z;

    bScale.x = 0.15;
    bScale.y = 0.15;
    bScale.z =1.0;

    bRot.x =0;
    bRot.y =0;
    bRot.z =90;  // depends on the image

    xMin = yMin =0;
    xMax = yMax =1.0;

    bLive = false;
    actionTrigger=IDLE;
}

void _bullets::bReset(vec3 pPos)
{
    bPos = pPos;
    bLive = false;
    actionTrigger = IDLE;
}

void _bullets::bUpdate(vec3 sPos, vec3 dPos)
{
    bActions();
    if(actionTrigger==SHOOT)
    {
        if(myTime->getTicks()>50)
        {
          /*  bPos.x= sPos.x +t*(dPos.x-sPos.x);
            bPos.y= sPos.y +t*(dPos.y-sPos.y);

            if(t<1) t+=0.05;
            else{t=0; actionTrigger =0;}
           */

           bPos.x +=0.1;

           if(bPos.x >bDes.x)
            bReset(sPos);

          myTime->reset();
        }
    }
}

void _bullets::bActions()
{
    switch(actionTrigger)
    {
        case IDLE: bLive=false; break; //idle
        case SHOOT: bLive=true; break; //shoot
        case HIT: bLive=false;  // TODO : explosion and reset
        break; //hit
    }
}

void _bullets::drawBullet(GLuint TX)
{
    glPushMatrix();


    if(bLive){
       glTranslatef(bPos.x,bPos.y,bPos.z);
       glScalef(bScale.x,bScale.y,bScale.z);

       glRotatef(bRot.x,1,0,0);
       glRotatef(bRot.y,0,1,0);
       glRotatef(bRot.z,0,0,1);

       glBindTexture(GL_TEXTURE_2D,TX);

       glBegin(GL_QUADS);
         glTexCoord2f(xMin,yMax);
         glVertex3f(-1,-1,0);
         glTexCoord2f(xMax,yMax);
         glVertex3f( 1,-1,0);
         glTexCoord2f(xMax,yMin);
         glVertex3f( 1,1,0);
         glTexCoord2f(xMin,yMin);
         glVertex3f(-1,1,0);
       glEnd();
    }
    glPopMatrix();
}
