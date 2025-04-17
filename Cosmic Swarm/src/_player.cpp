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

    maxHp = currentHp = 100.0f;  // Initialize player health (adjust as needed)

    //xp
    playerLevel = 1;
    xpThresh = 5;      //initial xp to level up

    actionTrigger = IDLE;

    playerTextureLoader = make_shared<_textureLoader>();
    playerTextureLoader->loadTexture(fileName);
    if (!playerTextureLoader || playerTextureLoader->tex == 0) std::cout << "Player texture not loaded!" << std::endl;

    bulletTextureLoader = make_shared<_textureLoader>();
    bulletTextureLoader->loadTexture("images/Greenlasercannon.png");
    if (!bulletTextureLoader || bulletTextureLoader->tex == 0) std::cout << "Bullet texture not loaded!" << std::endl;

    rocketTex = make_shared<_textureLoader>();
    rocketTex->loadTexture("images/rocket.png");
    if (!rocketTex || rocketTex->tex == 0) std::cout << "Rocket texture not loaded!" << std::endl;

    laserTex = make_shared<_textureLoader>();
    laserTex->loadTexture("images/laser.png");
    if (!laserTex || laserTex->tex == 0) std::cout << "Laser texture not loaded!" << std::endl;

    flakTex = make_shared<_textureLoader>();
    flakTex->loadTexture("images/flak.png");
    if (!flakTex || flakTex->tex == 0) std::cout << "Flak texture not loaded!" << std::endl;

    /*
    energyTex = make_shared<_textureLoader>();
    energyTex->loadTexture("images/energy.png");
    */
    // Weapons
    Weapon defaultWeapon;
    defaultWeapon.init(DEFAULT, bulletTextureLoader, 5.0f, 2.0f, 1.0f, 30.0f);
    weapons.push_back(defaultWeapon);

    Weapon rocket;
    rocket.init(ROCKET, rocketTex, 10.0f, 2.0f, 0.5f,10.0f);
    rocket.isActive = true;
    weapons.push_back(rocket);

    Weapon laser;
    laser.init(LASER, laserTex, 20.0f, 0.1f, 1.0f, 0.0);
    laser.isActive = true;
    weapons.push_back(laser);

    Weapon flak;
    flak.init(FLAK, flakTex, 3.0f, 1.0f, 0.25f, 12.0f);
    flak.isActive = true;
    weapons.push_back(flak);

    /*
    Weapon energy;
    energy.init(ENERGY_FIELD, energyTex, 1.0f, 0.1f, 3.0f);
    energy.isActive = false;
    weapons.push_back(energy);
    */
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

        if (playerTextureLoader)
        {
            playerTextureLoader->textureBinder();
        }
        else
        {
            std::cout << "Null playerTextureLoader!" << std::endl;
        }


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

void _player::updateWeapons(float deltaTime, vector<_enemy>& enemies, vec3 mousePos, _sounds* sounds)
{
    static _Bullet laserBeam; // Persistent laser beam
    static bool laserInitialized = false;

    for (auto& weapon : weapons)
    {
        if (!weapon.isActive)
            continue;

        if (!weapon.texture)
        {
            std::cout << "Null texture in weapon!" << std::endl;
            continue;
        }

        weapon.timer.update(deltaTime);

        if (weapon.type == DEFAULT)
        {
            shoot(mousePos, sounds); // Mouse-directed default weapon
        }
        else if (weapon.type == LASER)
        {
            if (!laserInitialized)
            {
                // Initialize laser beam
                vec3 laserSpawnPos = playerPosition;
                laserBeam.init(laserSpawnPos, playerRotation, mousePos, weapon.texture, weapon);
                laserBeam.isAlive = true;
                laserInitialized = true;
                bullets.push_back(laserBeam);
            }
            // Update laser position and rotation every frame
            laserBeam.position = playerPosition;
            laserBeam.rotation = playerRotation;
            laserBeam.isAlive = true; // Ensure laser remains active
            for (auto& bullet : bullets)
            {
                if (bullet.weapon.type == LASER)
                {
                    bullet = laserBeam;
                    break;
                }
            }
        }
        else
        {
            _enemy* target = nullptr;

            if (weapon.type == FLAK)
            {
                // Find nearest enemy for flak
                float minDistSq = std::numeric_limits<float>::max();
                for (auto& enemy : enemies)
                {
                    if (!enemy.isAlive) continue;
                    float dx = enemy.position.x - playerPosition.x;
                    float dy = enemy.position.y - playerPosition.y;
                    float distSq = dx * dx + dy * dy;
                    if (distSq < minDistSq) {
                        minDistSq = distSq;
                        target = &enemy;
                    }
                }
            }
            else if (weapon.type == ROCKET)
            {
                // Find enemy with most neighbors within 5.0f units (adjustable)
                target = findMostClusteredEnemy(enemies, 5.0f);
            }

            if (target && weapon.canFire())
            {
                vec3 targetPos = target->position;
                float angle = atan2(targetPos.y - playerPosition.y, targetPos.x - playerPosition.x) * (180.0f / M_PI) - 90.0f;

                if (weapon.type == ROCKET)
                {
                    _Bullet bullet;
                    bullet.init(playerPosition, {0, 0, angle}, targetPos, weapon.texture, weapon);
                    bullet.isAlive = true;
                    bullets.push_back(bullet);
                }
                else if (weapon.type == FLAK)
                {
                    // Increase number of bullets and spread
                    int numBullets = static_cast<int>(weapon.level + 3.0f); // More bullets
                    float totalSpread = 45.0f; // Wider spread (45 degrees)
                    float spreadPerBullet = numBullets > 1 ? totalSpread / (numBullets - 1) : 0.0f;

                    for (int i = 0; i < numBullets; i++)
                    {
                        // Calculate spread angle
                        float spreadAngle = angle - (totalSpread / 2.0f) + (i * spreadPerBullet);
                        // Add slight random offset (±3 degrees)
                        float randomOffset = (rand() % 600 - 300) / 100.0f; // -3.0 to 3.0
                        _Bullet bullet;
                        bullet.init(playerPosition, {0, 0, spreadAngle + randomOffset}, targetPos, weapon.texture, weapon);
                        bullet.isAlive = true;
                        bullets.push_back(bullet);
                    }
                }
                else if (weapon.type == ENERGY_FIELD)
                {
                    _Bullet bullet;
                    bullet.init(playerPosition, {0, 0, 0}, playerPosition, weapon.texture, weapon);
                    bullet.isAlive = true;
                    bullets.push_back(bullet);
                }
                weapon.timer.reset();
            }
        }
    }

    // Update bullets
    for (auto& bullet : bullets)
    {
        if (bullet.isAlive)
        {
            bullet.update(deltaTime, enemies);
        }
    }
}

void _player::shoot(vec3 mousePos, _sounds *sounds)
{
    Weapon& defaultWeapon = weapons[0]; // Default weapon is first in the vector
    if (!defaultWeapon.texture) std::cout << "Default weapon texture is null!" << std::endl;

    if (defaultWeapon.canFire())
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
        leftBullet.init(leftSpawnPos, playerRotation, mousePos, defaultWeapon.texture, defaultWeapon);
        leftBullet.actionTrigger = _Bullet::SHOOT;
        leftBullet.isAlive = true;

        _Bullet rightBullet;
        rightBullet.init(rightSpawnPos, playerRotation, mousePos, defaultWeapon.texture, defaultWeapon);
        rightBullet.actionTrigger = _Bullet::SHOOT;
        rightBullet.isAlive = true;

        bullets.push_back(leftBullet);
        bullets.push_back(rightBullet);

        defaultWeapon.timer.reset(); // Reset weapon timer
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
            playerTimer->reset();

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
                xMax = 1.0f / 6.0f;
            }
            else    // white sprite for flash
            {
                xMin = 3.0f/6.0f;
                xMax = 4.0f/6.0f;
            }
        }
        break;
        case FLYING:
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

            if (playerTimer->getTicks() < 250)  // First 100ms: Thruster startup frame
            {
                if(!startFlash) // default sprite
                {
                    xMin = 1.0f/6.0f;
                    xMax = 2.0f/6.0f;
                }
                else    // white sprite for flash
                {
                    xMin = 4.0f/6.0f;
                    xMax = 5.0f/6.0f;
                }
            }
            else  // After 100ms: Fully engaged thrusters
            {
                if(!startFlash) // default sprite
                {
                    xMin = 2.0f/6.0f;
                    xMax = 3.0f/6.0f;
                }
                else    // white sprite for flash
                {
                    xMin = 5.0f/6.0f;
                    xMax = 6.0f/6.0f;
                }
            }

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
    startFlash = true;
    flashTimer = 0.0f;  // Reset flash timer
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

void _player::gainXP(int amount)
{
    experiencePoints += amount;

    if(experiencePoints >= xpThresh)
    {
        experiencePoints -= xpThresh;
        playerLevel++;
        xpThresh += 20;     //change this for xp increase per level
    }
}

void _player::drawXPBar()
{
    float barWidth = 300.0f;
    float barHeight = 20.0f;
    float margin = 20.0f;
    float filledWidth = (float)experiencePoints / xpThresh * barWidth;

    float x = margin;
    float y = margin;

    glPushMatrix();
    glDisable(GL_TEXTURE_2D);

    // Background
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + barWidth, y);
        glVertex2f(x + barWidth, y + barHeight);
        glVertex2f(x, y + barHeight);
    glEnd();

    // Fill
    glColor3f(0.573f, 0.878f, 0.914f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + filledWidth, y);
        glVertex2f(x + filledWidth, y + barHeight);
        glVertex2f(x, y + barHeight);
    glEnd();

    // Draw Level Text
    string levelText = "Level " + to_string(playerLevel);
    float estimatedWidth = levelText.length() * 10.0f;
    float textX = x + (barWidth / 2.0f) - (estimatedWidth / 2.0f);
    float textY = y + 4;

    glColor3f(1.0f, 1.0f, 1.0f);  // White text
    glRasterPos2f(textX, textY);
    for (char c : levelText)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

void _player::drawHealthBar()
{
    float barWidth = 300.0f;
    float barHeight = 20.0f;
    float margin = 20.0f;
    float gapAboveXP = 15.0f;

    float filledWidth = (currentHp / maxHp) * barWidth;

    float x = margin;
    float y = margin + barHeight + gapAboveXP;  // Positioned above XP bar

    glPushMatrix();
    glDisable(GL_TEXTURE_2D);

    // Background (gray)
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + barWidth, y);
        glVertex2f(x + barWidth, y + barHeight);
        glVertex2f(x, y + barHeight);
    glEnd();

    // Health fill (red)
    glColor3f(0.937f, 0.541f, 0.196f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + filledWidth, y);
        glVertex2f(x + filledWidth, y + barHeight);
        glVertex2f(x, y + barHeight);
    glEnd();

    //Draw HP label above the bar
    string hpText = "HP: " + to_string((int)currentHp) + " / " + to_string((int)maxHp);
    float textWidth = hpText.length() * 10.0f;
    float textX = x + (barWidth - textWidth) / 2.0f;
    float textY = y + barHeight + 15.0f;

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(textX, textY);
    for (char c : hpText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

_enemy* _player::findMostClusteredEnemy(vector<_enemy>& enemies, float clusterRadius)
{
    _enemy* bestTarget = nullptr;
    int maxNeighbors = -1;
    float minDistSq = std::numeric_limits<float>::max(); // For tie-breaking by distance

    for (auto& enemy : enemies) {
        if (!enemy.isAlive) continue;

        // Count enemies within clusterRadius
        int neighborCount = 0;
        for (auto& other : enemies) {
            if (!other.isAlive || &other == &enemy) continue;
            float dx = enemy.position.x - other.position.x;
            float dy = enemy.position.y - other.position.y;
            float distSq = dx * dx + dy * dy;
            if (distSq <= clusterRadius * clusterRadius) {
                neighborCount++;
            }
        }

        // Update best target if this enemy has more neighbors or equal neighbors but is closer
        float dxToPlayer = enemy.position.x - playerPosition.x;
        float dyToPlayer = enemy.position.y - playerPosition.y;
        float distToPlayerSq = dxToPlayer * dxToPlayer + dyToPlayer * dyToPlayer;

        if (neighborCount > maxNeighbors || (neighborCount == maxNeighbors && distToPlayerSq < minDistSq)) {
            maxNeighbors = neighborCount;
            bestTarget = &enemy;
            minDistSq = distToPlayerSq;
        }
    }

    return bestTarget;
}
