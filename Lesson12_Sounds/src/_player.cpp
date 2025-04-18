#include "_player.h"

_player::_player()
{

}
_player::~_player()
{

}

void _player::initPlayer(int xFrames, int yFrames, char* idle, char* run, char * attack)
{
    // INIT QUAD
    vertices[0].x = -1.0; vertices[0].y = -1.0; vertices[0].z = -1.0;
    vertices[1].x = 1.0; vertices[1].y = -1.0; vertices[1].z = -1.0;
    vertices[2].x = 1.0; vertices[2].y = 1.0; vertices[2].z = -1.0;
    vertices[3].x = -1.0; vertices[3].y = 1.0; vertices[3].z = -1.0;

    // INIT PLAYER POS/SCALE
    playerPosition.x = 0.0; playerPosition.y = -0.65; playerPosition.z = -2.0;
    playerScale.x = playerScale.y = playerScale.z = 0.65;

    // GET SPRITE FRAMES
    framesX=xFrames;
    framesY=yFrames;

    // INIT ACTION
    actionTrigger = 0;
    facingDirection = 1;

    // INIT TEXTURE
    xMin =5.0/(float)framesX;
    xMax = 1.0;
    yMax = 1.0/(float)framesY;
    yMin = yMax - (1.0/(float)framesY);
    playerTextureLoader->loadTexture(idle);

    playerIdle = idle;
    playerRun = run;
    playerAttack = attack;

    actionTrigger = IDLE;
}

void _player::drawPlayer()
{
    glPushMatrix();
        glTranslatef(playerPosition.x,playerPosition.y,playerPosition.z);
        glScalef(playerScale.x,playerScale.y,playerScale.z);
        glColor3f(1.0,1.0,1.0);
        playerTextureLoader->textureBinder();

        glBegin(GL_QUADS);
        glTexCoord2f(xMin,yMax);
        glVertex3f(vertices[0].x,vertices[0].y,vertices[0].z);

        glTexCoord2f(xMax,yMax);
        glVertex3f(vertices[1].x,vertices[1].y,vertices[1].z);

        glTexCoord2f(xMax,yMin);
        glVertex3f(vertices[2].x,vertices[2].y,vertices[2].z);

        glTexCoord2f(xMin,yMin);
        glVertex3f(vertices[3].x,vertices[3].y,vertices[3].z);
        glEnd();

    glPopMatrix();
}

void _player::setFacingDirection(int direction)
{
    if(facingDirection==direction)
    {
        return;
    }

    facingDirection = direction;

    playerScale.x *= -1;
}

void _player::playerActions()
{
    switch(actionTrigger)
    {
        case IDLE:
        {
            xMin =5.0/(float)framesX;
            xMax = 1.0;
            yMax = 1.0/(float)framesY;
            yMin = yMax - (1.0/(float)framesY);
        }
            break;
        case RUN:
        {
            if(playerTimer->getTicks() > 50)
            {
                xMax +=1.0/(float)framesX;
                xMin += 1.0/(float)framesX;

                if (xMax >= 1.0)
                {
                    xMax = 1.0 / (float)framesX;
                    xMin = 0;

                    yMax += 1.0 / (float)framesY;
                    yMin += 1.0 / (float)framesY;

                    if (yMax > 1.0)
                    if (yMax > 1.0)
                    {
                        yMin = 0;
                        yMax = 1.0 / (float)framesY;
                    }
                }
                playerTimer->reset();
            }
            break;
        }
        case WALK_BACKWARD:
        {
            if (playerTimer->getTicks() > 100) // Slower animation for walking backward
            {
                xMax -= 1.0 / (float)framesX; // Reverse order
                xMin -= 1.0 / (float)framesX;

                if (xMin <= 0)
                {
                    xMin = (float)(framesX - 1) / framesX; // Start from last frame
                    xMax = 1.0;

                    yMax += 1.0 / (float)framesY;
                    yMin += 1.0 / (float)framesY;

                    if (yMax > 1.0)
                    {
                        yMin = 0;
                        yMax = 1.0 / (float)framesY;
                    }
                }
                playerTimer->reset();
            }
            break;
        }
    }
}

