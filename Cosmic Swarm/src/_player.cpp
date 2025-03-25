#include "_player.h"

_player::_player()
{

}
_player::~_player()
{

}
void _player::initPlayer(int xFrames, int yFrames, char* fileName)
{
    // INIT QUAD
    vertices[0].x = -1.0; vertices[0].y = -1.0; vertices[0].z = -1.0;
    vertices[1].x = 1.0; vertices[1].y = -1.0; vertices[1].z = -1.0;
    vertices[2].x = 1.0; vertices[2].y = 1.0; vertices[2].z = -1.0;
    vertices[3].x = -1.0; vertices[3].y = 1.0; vertices[3].z = -1.0;

    // INIT PLAYER POS/SCALE
    playerPosition.x = 0.0; playerPosition.y = -0.65; playerPosition.z = 50;
    playerScale.x = playerScale.y = playerScale.z = 1;
    playerRotation = {0,0,0};

    // GET SPRITE FRAMES
    framesX=xFrames;
    framesY=yFrames;

    // INIT ACTION
    actionTrigger = 0;

    // INIT TEXTURE
    xMin = 0;
    xMax = 1.0;
    yMax = 1.0;
    yMin = 0;
    speed = 10.0f;
    playerTextureLoader->loadTexture(fileName);

    actionTrigger = IDLE;
}

void _player::drawPlayer()
{
    glPushMatrix();
        glTranslatef(playerPosition.x,playerPosition.y,playerPosition.z);
        glScalef(playerScale.x,playerScale.y,playerScale.z);
        glRotatef(playerRotation.x, 1, 0, 0);
        glRotatef(playerRotation.y, 0, 1, 0);
        glRotatef(playerRotation.z, 0, 0, 1);

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

void _player::shoot(vec3 mousePos)
{
    if (bulletTimer.getTicks() > 250)
    {
        _Bullet newBullet;
        newBullet.init(playerPosition,playerRotation,mousePos,"images/Greenlasercannon.png");
        newBullet.actionTrigger = _Bullet::SHOOT;
        newBullet.isAlive = true;
        bullets.push_back(newBullet);
        bulletTimer.reset();
    }
}

void _player::playerActions()
{
    switch(actionTrigger)
    {
        case IDLE:
        {
            xMin = 0.0f;
            xMax = 1.0f / 3.0f;
            yMin = 0.0f;
            yMax = 1.0f;
            playerTimer->reset();
        }
        break;
        case FLYING:
        {
            if (playerTimer->getTicks() < 250)  // First 100ms: Thruster startup frame
            {
                xMin = 1.0f / 3.0f;  // Second frame (thrusters starting)
                xMax = 2.0f / 3.0f;
            }
            else  // After 100ms: Fully engaged thrusters
            {
                xMin = 2.0f / 3.0f;  // Third frame (fully engaged)
                xMax = 1.0f;
            }

            yMin = 0.0f;
            yMax = 1.0f;
        }
        break;

    }
}

