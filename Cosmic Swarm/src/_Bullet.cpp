#include "_Bullet.h"

_Bullet::_Bullet()
{

}

_Bullet::~_Bullet()
{

}

void _Bullet::init(vec3 playerPos, vec3 playerRotation, vec3 targetPos, char * fileName, bool isLeft)
{
    initialPosition = position = playerPos;
    position.z = playerPos.z + 1;  // Ensure bullets spawn in front of the player

    float angleRad = (playerRotation.z + 90) * (M_PI / 180.0); // Convert to radians
    direction.x = cos(angleRad);
    direction.y = sin(angleRad);

    scale = {1, 1, 1};
    rotation = playerRotation;

    xMin = yMin = 0;
    xMax = yMax = 1.0;

    isLeftBullet = isLeft;
    // Define offset based on whether it's left or right bullet
    float offsetDistance = 0.5f;  // Adjust this value based on your bullet sprite size/position
    offset.x = isLeft ? -offsetDistance : offsetDistance;
    offset.y = 0.0f;
    offset.z = 0.0f;

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

    float maxTravelDistance = 100.0f;  // Set a maximum range for bullets
    float distanceSq = (position.x - initialPosition.x) * (position.x - initialPosition.x) +
                       (position.y - initialPosition.y) * (position.y - initialPosition.y);

    if (distanceSq > maxTravelDistance * maxTravelDistance)
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
