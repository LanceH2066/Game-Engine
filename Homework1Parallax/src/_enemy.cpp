#include "_enemy.h"

_enemy::_enemy()
{
    position = {0,0,-2};
    scale = {0.25,0.25};
    rotation = {0,0,0};
    actionTrigger = 0;
    speed = 0.1f;
    isAlive = true;
}

_enemy::~_enemy()
{

}
void _enemy::initEnemy(int xFrames, int yFrames, char* fileName)
{
    framesX=xFrames;
    framesY=yFrames;

    xMin = 0;
    xMax = 1.0/(float)framesX;
    yMax = 1.0/(float)framesY;
    yMin = yMax-(1.0/framesY);

    enemyTextureLoader->loadTexture(fileName);
}

void _enemy::drawEnemy()
{
    if(isAlive)
    {
        glPushMatrix();

            glColor3f(1.0,1.0,1.0);
            enemyTextureLoader->textureBinder();

            glTranslatef(position.x,position.y,position.z);
            glRotatef(rotation.x,1,0,0);
            glRotatef(rotation.y,0,1,0);
            glRotatef(rotation.z,0,0,1);
            glScalef(scale.x,scale.y,1.0);

            glBegin(GL_POLYGON);

            glTexCoord2f(xMin,yMin);
            glVertex3f(1.0,1.0,0);

            glTexCoord2f(xMax,yMin);
            glVertex3f(-1.0,1.0,0);

            glTexCoord2f(xMax,yMax);
            glVertex3f(-1.0,-1.0,0);

            glTexCoord2f(xMin,yMax);
            glVertex3f(1.0,-1.0,0);

            glEnd();

        glPopMatrix();
    }
}

void _enemy::placeEnemy()
{

}

void _enemy::enemyActions()
{
    switch(actionTrigger)
    {
    case 0:
        if(enemyTimer->getTicks()>60)
        {
            xMin += 1.0/(float)framesX;
            xMax += 1.0/(float)framesX;

            yMin = 0;
            yMax = 0.5;


            if(position.x >= -1)
            {
               position.x += speed * -1.0;
            }
            else
            {
                actionTrigger = 1;
                break;
            }

            enemyTimer->reset();
        }
        break;
    case 1:
    if(enemyTimer->getTicks()>60)
    {
        yMin = 0.5;
        yMax = 1;

        xMin += 1.0/(float)framesX;
        xMax += 1.0/(float)framesX;

        if(position.x <= 1)
        {
            position.x += speed * 1.0;;
        }
        else
        {
            actionTrigger = 0;
            break;
        }

        enemyTimer->reset();
    }
    break;
    }
}
