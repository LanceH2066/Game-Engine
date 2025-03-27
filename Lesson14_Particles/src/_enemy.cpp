#include "_enemy.h"

_enemy::_enemy()
{
    position = {0,0,-2};
    scale = {0.25,0.25};
    rotation = {0,0,0};
    actionTrigger = IDLE;
    speed = 0.05f;
    isAlive = true;

    framesX=7;
    framesY=2;

    xMin = 0;
    xMax = 1.0/(float)framesX;
    yMax = 1.0/(float)framesY;
    yMin = yMax-(1.0/framesY);

}

_enemy::~_enemy()
{

}
void _enemy::initEnemy(char* fileName)
{
    enemyTextureLoader->loadTexture(fileName);
}

void _enemy::drawEnemy(GLuint tex)
{
    if(isAlive)
    {
        glPushMatrix();

            glColor3f(1.0,1.0,1.0);
            //enemyTextureLoader->textureBinder();
            glBindTexture(GL_TEXTURE_2D, tex);

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

void _enemy::placeEnemy(vec3 pos)
{
    position = pos;
}

void _enemy::enemyActions()
{
    switch(actionTrigger)
    {
    case IDLE:
        if(enemyTimer->getTicks()>60)
        {
            xMin = 0;
            xMax = 1.0/(float)framesX;
            xMin += 1.0/(float)framesX;
            xMax += 1.0/(float)framesX;

            if(position.x < 0)
            {
                actionTrigger = RIGHTWALK;
            }
            else
            {
                actionTrigger = LEFTWALK;
            }
            enemyTimer->reset();
        }
    break;
    case LEFTWALK:
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
                actionTrigger = RIGHTWALK;
                break;
            }

            enemyTimer->reset();
        }
        break;
    case RIGHTWALK:
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
            actionTrigger = LEFTWALK;
            break;
        }

        enemyTimer->reset();
    }
    break;
    case ROTATELEFT:
    if(enemyTimer->getTicks()>60)
    {
        yMin = 0.5;
        yMax = 1;
        xMin += 1.0/(float)framesX;
        xMax += 1.0/(float)framesX;

        theta = 30*PI/180;
        position.x -= v*t*cos(theta)/1500;
        position.y += (v*t*sin(theta)-0.5*GRAVITY*t*t)/200;

        if(position.y > -0.8)
        {
            t+=0.3;
        }
        else
        {
            t=0;
            position.y=-0.7;
        }

        if(position.x < -3.0)
        {
            position.y = -0.8;
            actionTrigger = RIGHTWALK;
        }

        enemyTimer->reset();
    }
    break;
    case ROTATERIGHT:
    if(enemyTimer->getTicks()>60)
    {
        xMin += 1.0/(float)framesX;
        xMax += 1.0/(float)framesX;
        yMin = 0;
        yMax = 0.5;

        theta = 30*PI/180;
        position.x += v*t*cos(theta)/1500;
        position.y += (v*t*sin(theta)-0.5*GRAVITY*t*t)/200;

        if(position.y > -0.8)
        {
            t+=0.3;
        }
        else
        {
            t=0;
            position.y=-0.7;
        }

        if(position.x > 3.0)
        {
            position.y = -0.8;
            actionTrigger = LEFTWALK;
        }

        enemyTimer->reset();
    }
    break;
    }
}
