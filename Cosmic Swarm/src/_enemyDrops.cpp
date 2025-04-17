#include "_enemyDrops.h"

_enemyDrops::_enemyDrops()
{
    //ctor
    position = vec3(0.0f, 0.0f, 0.0f);
    scale = vec3(0.6f, 0.6f, 1.0f);
    isActive = true;

    xMin = 0.0f;
    xMax = 1.0f;
    yMin = 0.0f;
    yMax = 1.0f;
}

_enemyDrops::~_enemyDrops()
{
    //dtor
}

void _enemyDrops::initDrop(dropType t, const char* filePath)
{
    type = t;
    dropTextureLoader->loadTexture((char*)filePath);
}

void _enemyDrops::drawDrop()
{
    if (!isActive) return;

    glPushMatrix();

        glTranslatef(position.x, position.y, position.z);
        glScalef(scale.x, scale.y, 1.0f);
        glColor3f(1.0f, 1.0f, 1.0f);

        dropTextureLoader->textureBinder();

        glBegin(GL_QUADS);
            glTexCoord2f(xMin, yMin); glVertex3f(-1.0f, -1.0f, 0.0f);
            glTexCoord2f(xMax, yMin); glVertex3f(1.0f, -1.0f, 0.0f);
            glTexCoord2f(xMax, yMax); glVertex3f(1.0f, 1.0f, 0.0f);
            glTexCoord2f(xMin, yMax); glVertex3f(-1.0f, 1.0f, 0.0f);
        glEnd();

    glPopMatrix();
}

void _enemyDrops::placeDrop(vec3 pos)
{
    position = pos;
}

void _enemyDrops::update(float deltaTime, vec3 playerPos, float& xpPickupRange, bool& magnetActive, float& currentHp, float maxHp)
{
    if (!isActive && !magnetTriggered) return;

    timeAlive += deltaTime;

    // Check pickup
    if (isActive)
    {
        vec3 diff = {
            playerPos.x - position.x,
            playerPos.y - position.y,
            0.0f
        };

        float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
        float pickupRange = 1.0f;

        if (dist <= pickupRange)
        {
            isActive = false;

            switch (type)
            {
                case MAGNET:
                    xpPickupRange = 1000.0f;
                    magnetActive = true;
                    magnetTimer = magnetDuration;
                    magnetTriggered = true;
                    break;

                case HEALTH:
                    currentHp = std::min(maxHp, currentHp + 50.0f); //change value for amount of health recovered
                    break;

                case SHIELD:
                    // Add shield later
                    break;
            }
        }
    }

    // Handle magnet timer if activated
    if (magnetTriggered)
    {
        magnetTimer -= deltaTime;
        if (magnetTimer <= 0.0f)
        {
            xpPickupRange = 5.0f;   // Reset to default
            magnetActive = false;
            magnetTriggered = false;
        }
    }

    if (timeAlive >= maxLifetime && !magnetTriggered)
    {
        isActive = false;
    }
}


vec3 _enemyDrops::getCollisionBoxMin() const
{
    return {
        position.x - collisionBoxSize.x,
        position.y - collisionBoxSize.y,
        position.z - collisionBoxSize.z
    };
}

vec3 _enemyDrops::getCollisionBoxMax() const
{
     return {
        position.x + collisionBoxSize.x,
        position.y + collisionBoxSize.y,
        position.z + collisionBoxSize.z
    };
}
