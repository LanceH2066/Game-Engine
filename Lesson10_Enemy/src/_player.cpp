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
    playerPosition.x = 0.0; playerPosition.y = 0.0; playerPosition.z = -2.0;
    playerScale.x = playerScale.y = playerScale.z = 1.0;

    // GET SPRITE FRAMES
    framesX=xFrames;
    framesY=yFrames;

    // INIT ACTION
    actionTrigger = 0;

    // INIT TEXTURE
    xMin = 0;
    xMax = 1.0/(float)framesX;
    yMax = 1.0/(float)framesY;
    yMin = yMax-(1.0/framesY);
    playerTextureLoader->loadTexture(idle);

    playerIdle = idle;
    playerRun = run;
    playerAttack = attack;
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

void _player::playerActions()
{
    switch(actionTrigger)
    {
        case IDLE:
            if(playerTimer->getTicks() > 70)
            {
                xMax +=1.0/(float)framesX;
                xMin += 1.0/(float)framesX;
                playerTimer->reset();
            }
            break;
        case LEFTWALK:
            if(playerTimer->getTicks() > 70)
            {
                xMax +=1.0/(float)framesX;
                xMin += 1.0/(float)framesX;
                playerTimer->reset();
            }
            break;
    }
}

