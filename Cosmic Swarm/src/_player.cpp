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
    facingDirection = 1;

    // INIT TEXTURE
    xMin = 0;
    xMax = 1.0;
    yMax = 1.0;
    yMin = 0;
    speed = 1.0f;
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

void _player::setFacingDirection(const string& direction)
{
    // Check if direction is already set
    if (facingDirection == direction)
        return;

    // Update facing direction
    facingDirection = direction;

    // Adjust scale/rotation for proper orientation
    if (direction == "left")
    {
        playerRotation.z = 90;
        playerRotation.x = 0;
    }
    else if (direction == "right")
    {
        playerRotation.z = -90;
        playerRotation.x = 0;
    }
    else if (direction == "up")
    {
        playerRotation.x = 0;
        playerRotation.z = 0;
    }
    else if (direction == "down")
    {
        playerRotation.x = 180;
        playerRotation.z = 0;
    }
}

void _player::playerActions()
{
    switch(actionTrigger)
    {
        case IDLE:
        {
            xMin =0;
            xMax = 1.0;
            yMax = 1.0;
            yMin = 0;
        }
            break;
        case FLYING:
        {
            if(playerTimer->getTicks() > 10)
            {
                if (facingDirection == "left")
                {
                    playerPosition.x -= speed;
                }
                else if (facingDirection == "right")
                {
                    playerPosition.x += speed;
                }
                else if (facingDirection == "up")
                {
                    playerPosition.y += speed;
                }
                else if (facingDirection == "down")
                {
                    playerPosition.y -= speed;
                }

            /*
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
            */
                playerTimer->reset();
            }
            break;
        }
    }
}

