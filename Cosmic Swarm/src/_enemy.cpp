#include "_enemy.h"

_enemy::_enemy()
{
    //position = {0,0,-2};
    position.x = 0.0; position.y = 0.0; position.z = -2.0;
    //scale = {5.0,5.0};
    scale.x = 1.0; scale.y = 1.0;
    //rotation = {0,0,0};
    rotation.x = 0.0; rotation.y = 0.0; rotation.z = 0.0;
    speed = 1.5f;
    isAlive = true;

    xMin = 0;
    xMax = 1.0;
    yMax = 1.0;
    yMin = 0;
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
    position.x = pos.x;
    position.y = pos.y;
    position.z = pos.z;
}

void _enemy::setPlayerReference(_player* player)
{
    targetPlayer = player;
}

void _enemy::enemyActions(float deltaTime)
{
    static float attackCooldown = 0.0f; // To track when the enemy can spray
    static float attackDuration = 0.0f; // To track how long the spray lasts

    switch(actionTrigger)
    {
        case PURSUIT:
        {
            if (targetPlayer && isAlive)
            {
                xMin = 0;
                xMax = 0.5f;

                // Compute direction vector from enemy to player
                float deltaX = targetPlayer->playerPosition.x - position.x;
                float deltaY = targetPlayer->playerPosition.y - position.y;

                // Compute the angle in degrees
                float angle = atan2(deltaY, deltaX) * (180.0f / M_PI);

                // Adjust to match enemy's default forward direction (downward)
                rotation.z = angle -90.0f;

                // Normalize direction vector
                float magnitude = sqrt(deltaX * deltaX + deltaY * deltaY);
                if (magnitude > 0.0f)
                {
                    deltaX /= magnitude;
                    deltaY /= magnitude;
                }

                // Move enemy towards the player
                position.x += deltaX * speed * deltaTime;
                position.y += deltaY * speed * deltaTime;

                enemyTimer->reset();
            }
            break;
        }
        case ATTACK:
        {
            // Attack cooldown logic: trigger attack every couple of seconds (e.g., 2 seconds)
            attackCooldown += deltaTime;
            if (attackCooldown >= 2.0f) // Every 2 seconds
            {
                // Activate the spray attack and show the spray animation for 1-2 seconds
                xMin = 0.5f;
                xMax = 1.0f;
                attackDuration = 1.5f; // Keep the spray attack on for 1.5 seconds

                attackCooldown = 0.0f; // Reset the cooldown timer
            }

            // Handle spray attack duration: keep it showing for a second or two
            if (attackDuration > 0.0f)
            {
                attackDuration -= deltaTime; // Reduce the duration
            }
            else
            {
                // After the duration ends, reset the animation back to idle state
                xMin = 0;
                xMax = 0.5f;
            }
            break;
        }
    }
}
