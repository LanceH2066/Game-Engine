#include "_enemy.h"
#include<_enemyDrops.h>

_enemy::_enemy()
{
    position.x = 0.0; position.y = 0.0; position.z = -2.0;
    scale.x = 1.0; scale.y = 1.0;
    rotation.x = 0.0; rotation.y = 0.0; rotation.z = 0.0;
    speed = 2.0f;
    isAlive = false;
    startFlash = false;
    xMin = 0;
    xMax = 1.0;
    yMax = 1.0;
    yMin = 0;
    currentHp = maxHp = 20.0f;
    explosionEffect = new _particleSystem();
    enemyTextureLoader = new _textureLoader(); // Initialize texture loader
    hasExploded = false;
    playerPosition = {0.0f, 0.0f, 0.0f}; // Initialize
}

_enemy::~_enemy()
{

}

void _enemy::initEnemy(char* fileName, float hp, vec3 hitboxSize, float mSpeed)
{
    enemyTextureLoader->loadTexture(fileName);
    explosionEffect->init("images/particle.png");
    maxHp = currentHp = hp; // Update HP
    collisionBoxSize = hitboxSize; // Update hitbox
    speed = mSpeed; // Update speed
}

void _enemy::drawEnemy(GLuint tex, float deltaTime)
{
    if(isAlive)
    {
        glPushMatrix();

            glColor3f(1.0,1.0,1.0);
            enemyTextureLoader->textureBinder();
            glTranslatef(position.x,position.y,position.z);
            glRotatef(rotation.x,1,0,0);
            glRotatef(rotation.y,0,1,0);
            glRotatef(rotation.z,0,0,1);
            glScalef(scale.x,scale.y,1.0);

            glBegin(GL_QUADS);

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
    if (explosionEffect->isActive())
    {
        explosionEffect->update(deltaTime);
        explosionEffect->draw();
    }
}

void _enemy::placeEnemy(vec3 pos)
{
    position.x = pos.x;
    position.y = pos.y;
    position.z = pos.z;
    hasExploded = false;
}

void _enemy::takeDamage(float damage,vector<_xpOrb>& xpOrbs, _textureLoader* xpOrbTexture, vector<_enemyDrops>& enemyDrops, _textureLoader* enemyDropsMagnetTexture, _textureLoader* enemyDropsHealthTexture)
{
    currentHp -= damage;
    if (currentHp <= 0 && isAlive)
    {
        isAlive = false;
        if (!hasExploded)
        {
            explosionEffect->spawnExplosion(position, 15, 0);
            hasExploded = true;
        }

        _xpOrb orb;
        orb.xpTextureLoader = xpOrbTexture;
        orb.placeOrb(position); // Use enemy's position
        orb.initOrb("images/xpOrb.png");
        xpOrbs.push_back(orb);

        if (rand() % 100 == 0)
        {
            _enemyDrops magnetDrop;
            magnetDrop.dropTextureLoader = enemyDropsMagnetTexture;
            magnetDrop.initDrop(_enemyDrops::MAGNET, "images/magnet.png");
            magnetDrop.placeDrop(position);
            enemyDrops.push_back(magnetDrop);
        }
        if (rand() % 70 == 0)
        {
            _enemyDrops healthDrop;
            healthDrop.initDrop(_enemyDrops::HEALTH, "images/healthDrop.png");
            healthDrop.placeDrop(position);
            enemyDrops.push_back(healthDrop);
        }
    }
    startFlash = true;
    flashTimer = 0.0f;  // Reset flash timer
}

void _enemy::enemyActions(float deltaTime)
{
    // Handle flash effect
    if (startFlash)
    {
        flashTimer += deltaTime;
        if (flashTimer >= flashDuration)
        {
            startFlash = false;  // Revert to default sprite
            flashTimer = 0.0f;  // Reset timer
        }
    }

    if(!startFlash) // default sprite
    {
        xMin = 0;
        xMax = 0.5f;
    }
    else    // white sprite for flash
    {
        xMin = 0.5f;
        xMax = 1.0f;
    }

    // Compute direction vector from enemy to player
    float deltaX = playerPosition.x - position.x;
    float deltaY = playerPosition.y - position.y;

    // Compute the angle in degrees
    float angle = atan2(deltaY, deltaX) * (180.0f / M_PI);

    // Adjust to match enemy's default forward direction (downward)
    rotation.z = angle - 90.0f;

    // Compute the distance to the player
    float magnitude = sqrt(deltaX * deltaX + deltaY * deltaY);

    // Normalize direction vector
    if (magnitude > 0.0f)
    {
        deltaX /= magnitude;
        deltaY /= magnitude;
    }

    // Move enemy towards the player only if outside the stopping distance
    if (magnitude > stoppingDistance)
    {
        position.x += deltaX * speed * deltaTime;
        position.y += deltaY * speed * deltaTime;
    }
}

vector<vec3> _enemy::getRotatedCorners() const
{
    vector<vec3> corners(4);
    vec3 min = getCollisionBoxMin();
    vec3 max = getCollisionBoxMax();

    corners[0] = {min.x - position.x, min.y - position.y, 0};  // Bottom-left
    corners[1] = {max.x - position.x, min.y - position.y, 0};  // Bottom-right
    corners[2] = {max.x - position.x, max.y - position.y, 0};  // Top-right
    corners[3] = {min.x - position.x, max.y - position.y, 0};  // Top-left

    float angleRad = rotation.z * (M_PI / 180.0);
    float cosA = cos(angleRad);
    float sinA = sin(angleRad);

    for (auto& corner : corners)
    {
        float x = corner.x * cosA - corner.y * sinA;
        float y = corner.x * sinA + corner.y * cosA;
        corner.x = x + position.x;
        corner.y = y + position.y;
    }

    return corners;
}
