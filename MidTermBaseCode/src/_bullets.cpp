#include "_bullets.h"

_bullets::_bullets()
{
    //ctor
}

_bullets::~_bullets()
{
    //dtor
}
void _bullets::bInit(vec3 pPos,_player* player)
{
    bPos = pPos;
    playerRef = player; // Store reference to player

    float angleRad = playerRef->plRot.z * (M_PI / 180.0f);
    float speed = 0.1;
    velocity.x = speed * cos(angleRad);
    velocity.y = speed * sin(angleRad);

    xMin = yMin =0;
    xMax = 0.5;
    yMax =1.0;

    bLive = false;
    actionTrigger = IDLE;
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
    if(actionTrigger == SHOOT)
    {
        if(myTime->getTicks() > 100)
        {
            // Convert cannon rotation (assumed to be `plRot.z`) to radians
            float angleRad = playerRef->plRot.z * (M_PI / 180.0f);

            // Set velocity using sine and cosine
            float speed = 0.1;
            bPos.x += speed * cos(angleRad);
            bPos.y += speed * sin(angleRad);

            // Check for out-of-bounds and reset if necessary
            if(bPos.x > bDes.x || bPos.y > bDes.y || bPos.x < 0 || bPos.y < 0)
            {
                bReset(sPos);
            }

            myTime->reset();
        }
    }
}

void _bullets::bActions()
{
    switch(actionTrigger)
    {
        case IDLE:
            bLive=false;
            xMin = 0;  // Reset to first frame
            xMax = 0.5;
            break; //idle
        case SHOOT:
        {
            bLive=true;
            if(myTime->getTicks() > 100)
            {
                // Toggle between two frames
                if (xMin == 0)
                {
                    xMin = 0.5;
                    xMax = 1.0;
                }
                else
                {
                    xMin = 0;
                    xMax = 0.5;
                }
            }
            break; //shoot
        }
        case HIT: bLive=false;  // TODO : explosion and reset
        break; //hit
    }
}

void _bullets::drawBullet(GLuint TX)
{
    glPushMatrix();


    if(bLive)
    {
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
