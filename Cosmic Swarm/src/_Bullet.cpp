#include "_Bullet.h"

_Bullet::_Bullet()
{

}

_Bullet::~_Bullet()
{

}

void _Bullet::init(vec3 playerPos)
{
    position = playerPos;
    position.z = -1.0 ;
    target.x = 5;               // for enemies this = playerpos , for players this will be mouse most likely
    target.y = playerPos.y;
    target.z = playerPos.z;
    scale = {0.15,0.15,1};
    rotation = {0,0,90};        // DEPENDS ON SPRITE

    xMin = yMin = 0;
    xMax = yMax = 1.0;
}

void _Bullet::reset(vec3 playerPos)
{
    position = playerPos;
    isAlive = false;
}

void _Bullet::update(vec3 source, vec3 target)
{
    if(actionTrigger == SHOOT)
    {
        if(timer->getTicks()>50)
        {
            position.x = source.x + t*(target.x-source.x);
            position.y = source.y + t*(target.y-source.y);

            if(t<1)
            {
                t+=0.05;
            }
            else
            {
                t = 0;
                actionTrigger = IDLE;
            }

            timer->reset();
        }
    }
}

void _Bullet::actions()
{
    switch(actionTrigger)
    {
        case IDLE:
        {
            isAlive = false;
            break;
        }
        case SHOOT:
        {
            isAlive = true;
            break;
        }
        case HIT:
        {
            isAlive = false;
            break;
        }
    }
}

void _Bullet::drawBullet(GLuint texture)
{
        glPushMatrix();
            if(isAlive)
            {
                glTranslatef(position.x,position.y,position.z);
                glScalef(scale.x,scale.y,scale.z);
                glRotatef(rotation.x,1,0,0);
                glRotatef(rotation.y,0,1,0);
                glRotatef(rotation.z,0,0,1);

                glBindTexture(GL_TEXTURE_2D, texture);

                glBegin(GL_QUADS);
                    glTexCoord2f(xMin,yMax);
                    glVertex3f(-1,-1,0);

                    glTexCoord2f(xMax,yMax);
                    glVertex3f(1,-1,0);

                    glTexCoord2f(xMax,yMin);
                    glVertex3f(1,1,0);

                    glTexCoord2f(xMin,yMin);
                    glVertex3f(-1,1,0);
                glEnd();
            }
        glPopMatrix();
}
