#include "_Bullet.h"

_Bullet::_Bullet()
{

}

_Bullet::~_Bullet()
{

}

void _Bullet::init(vec3 playerPos, vec3 playerRotation, vec3 targetPos, char * fileName)
{
    position = playerPos;
    position.z = playerPos.z + 1;  // Ensure bullets spawn in front of the player

    // Ensure bullets move straight out from the ship
    float angleRad = (playerRotation.z + 90) * (M_PI / 180.0); // Convert to radians

    // Set direction using player's rotation
    direction.x = cos(angleRad);
    direction.y = sin(angleRad);

    scale = {1, 1, 1};
    rotation = playerRotation;  // Bullet inherits player's rotation

    xMin = yMin = 0;
    xMax = yMax = 1.0;

    textureLoader->loadTexture(fileName);
}

void _Bullet::reset(vec3 playerPos)
{
    position = playerPos;
    isAlive = false;
}

void _Bullet::update(float deltaTime)
{
    float bulletSpeed = 30.0f; // Adjusted for frame-rate independence
    position.x += direction.x * bulletSpeed * deltaTime;
    position.y += direction.y * bulletSpeed * deltaTime;

    // If bullet moves off-screen, deactivate
    if (position.y > 100 || position.y < -100 || position.x > 100 || position.x < -100)
    {
        isAlive = false;
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

void _Bullet::drawBullet()
{
        glPushMatrix();
            if(isAlive)
            {
                glTranslatef(position.x,position.y,position.z);
                glScalef(scale.x,scale.y,scale.z);
                glRotatef(rotation.x,1,0,0);
                glRotatef(rotation.y,0,1,0);
                glRotatef(rotation.z,0,0,1);

                textureLoader->textureBinder();

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
