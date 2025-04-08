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

    actionTrigger = IDLE;
    playerTextureLoader->loadTexture(fileName);
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

void _player::shoot(vec3 mousePos, _sounds *sounds)
{
    if (bulletTimer.getTicks() > 250)
    {
        /*
        // Calculate spawn offsets based on player rotation
        float angleRad = playerRotation.z * (M_PI / 180.0);  // Player's facing angle
        float offsetDistance = 2.0f;  // Distance from center to left/right (tweak this)

        // Perpendicular offset (90 degrees from facing direction)
        float offsetX = cos(angleRad + M_PI / 2) * offsetDistance;
        float offsetY = sin(angleRad + M_PI / 2) * offsetDistance;

        // Left bullet spawn position
        vec3 leftSpawnPos = playerPosition;
        leftSpawnPos.x -= offsetX;  // Left side
        leftSpawnPos.y -= offsetY;

        // Right bullet spawn position
        vec3 rightSpawnPos = playerPosition;
        rightSpawnPos.x += offsetX;  // Right side
        rightSpawnPos.y += offsetY;
        */
        vec3 leftSpawnPos = playerPosition;
        vec3 rightSpawnPos = playerPosition;
        leftSpawnPos.x -= 0.5f;
        rightSpawnPos.x += 0.5f;

        // Create bullets
        _Bullet leftBullet;
        leftBullet.init(leftSpawnPos, playerRotation, mousePos, "images/Greenlasercannon.png");
        leftBullet.actionTrigger = _Bullet::SHOOT;
        leftBullet.isAlive = true;

        _Bullet rightBullet;
        rightBullet.init(rightSpawnPos, playerRotation, mousePos, "images/Greenlasercannon.png");
        rightBullet.actionTrigger = _Bullet::SHOOT;
        rightBullet.isAlive = true;

        bullets.push_back(leftBullet);
        bullets.push_back(rightBullet);

        bulletTimer.reset();
        sounds->playShootSound();
    }
}

void _player::playerActions(float deltaTime)
{
    playerTimer->update(deltaTime);
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

