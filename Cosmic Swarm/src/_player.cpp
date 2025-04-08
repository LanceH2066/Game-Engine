#include "_player.h"

_player::_player()
{
    maxHp = currentHp = 100.0f;  // Initialize player health (adjust as needed)
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
    bulletTextureLoader->loadTexture("images/Greenlasercannon.png");
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
        // Calculate spawn offsets based on player rotation
        float angleRad = playerRotation.z * (M_PI / 180.0);

        // Define gun positions relative to the ship's center (in local space)
        float gunOffsetX = 0.35f;  // Distance from center to each gun (left/right)
        float gunOffsetY = 0.5f;  // Forward offset (tweak based on sprite)

        // Transform gun offsets based on the ship's rotation
        float cosAngle = cos(angleRad);
        float sinAngle = sin(angleRad);

        // Left gun position (local space: (-gunOffsetX, gunOffsetY))
        float leftLocalX = -gunOffsetX;
        float leftLocalY = gunOffsetY;
        float leftWorldX = leftLocalX * cosAngle - leftLocalY * sinAngle;
        float leftWorldY = leftLocalX * sinAngle + leftLocalY * cosAngle;

        // Right gun position (local space: (gunOffsetX, gunOffsetY))
        float rightLocalX = gunOffsetX;
        float rightLocalY = gunOffsetY;
        float rightWorldX = rightLocalX * cosAngle - rightLocalY * sinAngle;
        float rightWorldY = rightLocalX * sinAngle + rightLocalY * cosAngle;

        // Apply to world positions
        vec3 leftSpawnPos = playerPosition;
        leftSpawnPos.x += leftWorldX;
        leftSpawnPos.y += leftWorldY;

        vec3 rightSpawnPos = playerPosition;
        rightSpawnPos.x += rightWorldX;
        rightSpawnPos.y += rightWorldY;

        // Create bullets
        _Bullet leftBullet;
        leftBullet.init(leftSpawnPos, playerRotation, mousePos, bulletTextureLoader);
        leftBullet.actionTrigger = _Bullet::SHOOT;
        leftBullet.isAlive = true;

        _Bullet rightBullet;
        rightBullet.init(rightSpawnPos, playerRotation, mousePos, bulletTextureLoader);
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

void _player::takeDamage(float damage)
{
    currentHp -= damage;
    if (currentHp <= 0)
    {
        currentHp = 0;  // Prevent negative health
        // Player is "dead" - we'll handle game-over in _scene
    }
}

vector<vec3> _player::getRotatedCorners() const
{
    vector<vec3> corners(4);
    vec3 min = getCollisionBoxMin();
    vec3 max = getCollisionBoxMax();

    corners[0] = {min.x - playerPosition.x, min.y - playerPosition.y, 0};  // Bottom-left
    corners[1] = {max.x - playerPosition.x, min.y - playerPosition.y, 0};  // Bottom-right
    corners[2] = {max.x - playerPosition.x, max.y - playerPosition.y, 0};  // Top-right
    corners[3] = {min.x - playerPosition.x, max.y - playerPosition.y, 0};  // Top-left

    float angleRad = playerRotation.z * (M_PI / 180.0);
    float cosA = cos(angleRad);
    float sinA = sin(angleRad);

    for (auto& corner : corners)
    {
        float x = corner.x * cosA - corner.y * sinA;
        float y = corner.x * sinA + corner.y * cosA;
        corner.x = x + playerPosition.x;
        corner.y = y + playerPosition.y;
    }

    return corners;
}
