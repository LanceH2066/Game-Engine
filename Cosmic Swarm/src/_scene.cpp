#include "_scene.h"

_scene::_scene(){
    input = new _inputs();
    prlx1 = new _parallax();
    player = new _player();
    collision = new _collision();
    sounds = new _sounds();
    xpOrbTexture = new _textureLoader();
    enemyDropsMagnetTexture = new _textureLoader();
    enemyDropsHealthTexture = new _textureLoader();
    hudTexture = new _textureLoader();
    fontTexture = new _textureLoader();
    damageIconTexture = new _textureLoader();
    fireRateIconTexture = new _textureLoader();
    aoeSizeIconTexture = new _textureLoader();
    speedIconTexture = new _textureLoader();
    healthIconTexture = new _textureLoader();
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);
    deltaTime = 0.0f;
    enemies.clear();
}

_scene::~_scene(){
    delete input;
    delete prlx1;
    delete player;
    delete collision;
    delete sounds;
}

GLint _scene::initGL(){
    srand(static_cast<unsigned>(time(nullptr)));
    // GL SETTINGS

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearDepth(1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // SCREEN SIZE
    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

    // INITIALIZE OBJECTS IN SCENE
    prlx1->initParallax("images/background.png", 0.005, false, false);
    player->initPlayer(1,1,"images/spritesheet.png");
    xpOrbTexture->loadTexture("images/xpOrb.png");
    enemyDropsMagnetTexture->loadTexture("images/magnet.png");
    enemyDropsHealthTexture->loadTexture("images/healthDrop.png");
    hudTexture->loadTexture("images/hud.png");
    fontTexture->loadTexture("images/font.png");
    loadFontData("images/font.fnt");
    damageIconTexture->loadTexture("images/damage.png");
    fireRateIconTexture->loadTexture("images/fireRate.png");
    aoeSizeIconTexture->loadTexture("images/aoeSize.png");
    speedIconTexture->loadTexture("images/speed.png");
    healthIconTexture->loadTexture("images/healthDrop.png");

    // Convert screen pixels to world space (assumes orthographic units)
    float worldUnitsPerPixel = 10.0f / dim.x; // Adjust based on projection settings
    float screenWidthUnits = dim.x * worldUnitsPerPixel;

    float minDistance = screenWidthUnits * 1.2f; // Slightly offscreen
    float maxDistance = screenWidthUnits * 1.5f; // More variation

    // INITIALIZE SOUNDS & START MUSIC
    sounds->playMusic();

    return true;
}

void _scene::drawScene(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    vec3 cameraPos = {player->playerPosition.x, player->playerPosition.y, player->playerPosition.z + 20.0f};
    vec3 lookAtPos = {player->playerPosition.x, player->playerPosition.y, player->playerPosition.z};
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
              lookAtPos.x, lookAtPos.y, lookAtPos.z,
              0, 1, 0);

    glPushMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        prlx1->drawBackground(dim.x, dim.y, player->playerPosition);
    glPopMatrix();


    glPushMatrix();
       player->drawPlayer();
    glPopMatrix();

    if (debugMode)
    {
        vec3 playerMin = player->getCollisionBoxMin();
        vec3 playerMax = player->getCollisionBoxMax();
        glPushMatrix();
            glDisable(GL_TEXTURE_2D);
            glTranslatef(player->playerPosition.x, player->playerPosition.y, player->playerPosition.z + 0.1f);
            glRotatef(player->playerRotation.z, 0, 0, 1);  // Apply player's rotation
            glColor3f(0.0f, 0.0f, 1.0f);  // Blue for player
            glBegin(GL_LINE_LOOP);
                glVertex3f(playerMin.x - player->playerPosition.x, playerMin.y - player->playerPosition.y, 0);
                glVertex3f(playerMax.x - player->playerPosition.x, playerMin.y - player->playerPosition.y, 0);
                glVertex3f(playerMax.x - player->playerPosition.x, playerMax.y - player->playerPosition.y, 0);
                glVertex3f(playerMin.x - player->playerPosition.x, playerMax.y - player->playerPosition.y, 0);
            glEnd();
            glColor3f(1.0f, 1.0f, 1.0f);
            glEnable(GL_TEXTURE_2D);
        glPopMatrix();
    }

    glPushMatrix();
        if (!isPaused)
        {
            player->updateWeapons(deltaTime, enemies, worldMousePos, sounds);
            player->bullets.erase
            (
                remove_if(player->bullets.begin(), player->bullets.end(), [](const _Bullet& b) { return !b.isAlive; }),
                player->bullets.end()
            );
        }
        for (auto& bullet : player->bullets)
        {
            if (bullet.isAlive)
            {
                bullet.drawBullet(deltaTime);

                vec3 bulletMin = bullet.getCollisionBoxMin();
                vec3 bulletMax = bullet.getCollisionBoxMax();

                if (bullet.weapon.type == LASER)
                {
                    // Check laser collision with target enemy
                    for (size_t i = 0; i < enemies.size(); i++)
                    {
                        if (collision->isOBBCollision(bullet, enemies[i]))
                        {
                            enemies[i].takeDamage(bullet.weapon.damage * deltaTime, xpOrbs, xpOrbTexture, enemyDrops, enemyDropsMagnetTexture, enemyDropsHealthTexture);
                        }
                    }
                }
                else if (bullet.weapon.type != ENERGY_FIELD)
                {
                // Handle default, rocket, flak collisions
                    for (size_t i = 0; i < enemies.size(); i++)
                    {
                        if (find(bullet.hitEnemies.begin(), bullet.hitEnemies.end(), i) == bullet.hitEnemies.end())
                        {
                            if (collision->isOBBCollision(bullet, enemies[i]))
                            {
                                if(bullet.weapon.type!= ROCKET)
                                {
                                    enemies[i].takeDamage(bullet.weapon.damage, xpOrbs, xpOrbTexture,enemyDrops, enemyDropsMagnetTexture, enemyDropsHealthTexture);
                                }
                                bullet.hitEnemies.push_back(i);
                                if (bullet.weapon.type != FLAK)
                                {
                                    if (bullet.weapon.type == ROCKET)
                                    {
                                        if(!bullet.hasExploded)
                                        {
                                            bullet.explode(enemies, xpOrbs, xpOrbTexture, enemyDrops, enemyDropsMagnetTexture, enemyDropsHealthTexture);
                                        }
                                    }
                                    // Only mark as dead if no explosion is active
                                    if (bullet.weapon.type != ROCKET || !bullet.explosionEffect->isActive())
                                    {
                                        bullet.isAlive = false;
                                    }
                                }

                            }
                        }
                    }
                }

                if (debugMode)
                {
                    glPushMatrix();
                        glDisable(GL_TEXTURE_2D);
                        glTranslatef(bullet.position.x, bullet.position.y, bullet.position.z + 0.1f);
                        glRotatef(bullet.rotation.z, 0, 0, 1);  // Apply bullet's rotation
                        glColor3f(0.0f, 1.0f, 0.0f);
                        glBegin(GL_LINE_LOOP);
                            glVertex3f(bulletMin.x - bullet.position.x, bulletMin.y - bullet.position.y, 0);
                            glVertex3f(bulletMax.x - bullet.position.x, bulletMin.y - bullet.position.y, 0);
                            glVertex3f(bulletMax.x - bullet.position.x, bulletMax.y - bullet.position.y, 0);
                            glVertex3f(bulletMin.x - bullet.position.x, bulletMax.y - bullet.position.y, 0);
                        glEnd();
                        glColor3f(1.0f, 1.0f, 1.0f);
                        glEnable(GL_TEXTURE_2D);
                    glPopMatrix();
                }
            }
        }
    glPopMatrix();

    if (!isPaused)
    {
        player->playerActions(deltaTime);
        updateEnemySpawning();
    }

    glPushMatrix();
        for (size_t i = 0; i < enemies.size(); i++)  // Use size()
        {
            if (!isPaused)
            {
                for (size_t j = i + 1; j < enemies.size(); j++)  // Use size()
                {
                    vec3 diff = {enemies[i].position.x - enemies[j].position.x, enemies[i].position.y - enemies[j].position.y, 0.0f};
                    float distSq = diff.x * diff.x + diff.y * diff.y;
                    float minDist = 0.5f;
                    if (distSq < (minDist * minDist) && distSq > 0.0f)
                    {
                        float dist = sqrt(distSq);
                        float pushForce = (minDist - dist) * 0.05f;
                        diff.x /= dist;
                        diff.y /= dist;
                        enemies[i].position.x += diff.x * pushForce;
                        enemies[i].position.y += diff.y * pushForce;
                        enemies[j].position.x -= diff.x * pushForce;
                        enemies[j].position.y -= diff.y * pushForce;
                    }
                }

                if (enemies[i].isAlive && player->currentHp > 0)
                {
                    enemies[i].playerPosition = player->playerPosition;
                    enemies[i].enemyActions(deltaTime);

                    if (collision->isOBBCollision(*player, enemies[i]))
                    {
                        if (damageCooldown <= 0.0f)
                        {
                            player->takeDamage(10.0f);  // Deal 10 damage per collision (adjust as needed)
                            damageCooldown = 0.5f;  // 0.5-second cooldown between damage (adjust as needed)
                            if (player->currentHp <= 0)
                            {
                                gameOver = true;  // Set game-over state
                                sounds->stopMusic();  // Stop music on game over
                            }
                        }
                    }
                }
            }

            enemies[i].drawEnemy(enemies[i].enemyTextureLoader->tex, deltaTime);

            if (debugMode && enemies[i].isAlive)
            {
                glPushMatrix();
                    glDisable(GL_TEXTURE_2D);
                    glTranslatef(enemies[i].position.x, enemies[i].position.y, enemies[i].position.z + 0.1f);
                    glRotatef(enemies[i].rotation.z, 0, 0, 1);  // Apply enemy's rotation
                    glColor3f(1.0f, 0.0f, 0.0f);
                    glBegin(GL_LINE_LOOP);
                        glVertex3f(enemies[i].getCollisionBoxMin().x - enemies[i].position.x, enemies[i].getCollisionBoxMin().y - enemies[i].position.y, 0);
                        glVertex3f(enemies[i].getCollisionBoxMax().x - enemies[i].position.x, enemies[i].getCollisionBoxMin().y - enemies[i].position.y, 0);
                        glVertex3f(enemies[i].getCollisionBoxMax().x - enemies[i].position.x, enemies[i].getCollisionBoxMax().y - enemies[i].position.y, 0);
                        glVertex3f(enemies[i].getCollisionBoxMin().x - enemies[i].position.x, enemies[i].getCollisionBoxMax().y - enemies[i].position.y, 0);
                    glEnd();
                    glColor3f(1.0f, 1.0f, 1.0f);
                    glEnable(GL_TEXTURE_2D);
                glPopMatrix();
            }
        }
    glPopMatrix();

    if (damageCooldown > 0.0f && !isPaused)
    {
        damageCooldown -= deltaTime;
        if (damageCooldown < 0.0f)
            damageCooldown = 0.0f;
    }

    for (auto& orb : xpOrbs)    //draw xp orbs
    {
        orb.update(deltaTime, player->playerPosition, player->xpPickupRange);
        orb.drawOrb();
    }

    //check for XP orb pickups
    for (auto& orb : xpOrbs)
    {
        if (orb.isActive && collision->isOBBCollision(*player, orb)) {
            orb.isActive = false;
            if (player->gainXP(1)) { // Check if player leveled up
                showUpgradeMenu();
            }
        }
    }

    //remove collected orbs
    xpOrbs.erase(
    remove_if(xpOrbs.begin(), xpOrbs.end(), [](const _xpOrb& o) { return !o.isActive; }),
    xpOrbs.end()
    );

    //enemy item drops
    for (auto& drop : enemyDrops)
    {
        drop.update(deltaTime, player->playerPosition, player->xpPickupRange, player->magnetActive, player->currentHp, player->maxHp);
        drop.drawDrop();
        if(drop.magnetTriggered == false)
        {
            player->magnetActive = false;
            player->xpPickupRange = 5.0f;
        }
    }
    // Remove inactive drops
    enemyDrops.erase(
    remove_if(enemyDrops.begin(), enemyDrops.end(), [](const _enemyDrops& d) { return !d.isActive; }),
    enemyDrops.end()
    );

    // Setup orthographic projection for 2D HUD
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, dim.x, 0, dim.y); // Pixel coordinates

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Reference resolution
    const float refWidth = 1920.0f;
    const float refHeight = 1080.0f;

    // Scaling factors
    float scaleX = dim.x / refWidth;
    float scaleY = dim.y / refHeight;

    // Time Display
    int minutes = static_cast<int>(elapsedTime) / 60;
    int seconds = static_cast<int>(elapsedTime) % 60;

    string timeText = (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
    float fontScale = scaleY * 2.0f;  // Adjust as needed
    float textX = (dim.x / 2.0f) - (timeText.length() * 20.0f * fontScale / 2.0f);
    float textY = dim.y - (90.0f * scaleY);  // Near top

    renderText(timeText, textX, textY, fontScale, 0.0f);

    // HP Bar (bottom left, 50px from left, 50px from bottom)
    float healthBarWidth = 425.0f * scaleX;
    float healthBarHeight = 80.0f * scaleY;
    float healthBarX = 0.0f;
    float healthBarY = 40.0f * scaleY;
    float healthFilledWidth = (player->currentHp / player->maxHp) * healthBarWidth;

    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    // Health fill (red)
    glColor3f(0.937f, 0.541f, 0.196f);
    glBegin(GL_QUADS);
        glVertex2f(healthBarX, healthBarY);
        glVertex2f(healthBarX + healthFilledWidth, healthBarY);
        glVertex2f(healthBarX + healthFilledWidth, healthBarY + healthBarHeight);
        glVertex2f(healthBarX, healthBarY + healthBarHeight);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();

    // XP Bar (bottom right, 50px from right, 50px from bottom)
    float xpBarWidth = 425.0f * scaleX;
    float xpBarHeight = 80.0f * scaleY;
    float xpBarX = dim.x - xpBarWidth;
    float xpBarY = 40.0f * scaleY;
    float xpFilledWidth = ((float)player->experiencePoints / player->xpThresh) * xpBarWidth;

    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    // Background (gray)
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(xpBarX, xpBarY);
        glVertex2f(xpBarX + xpBarWidth, xpBarY);
        glVertex2f(xpBarX + xpBarWidth, xpBarY + xpBarHeight);
        glVertex2f(xpBarX, xpBarY + xpBarHeight);
    glEnd();
    // XP fill (blue)
    glColor3f(0.573f, 0.878f, 0.914f);
    glBegin(GL_QUADS);
        glVertex2f(xpBarX, xpBarY);
        glVertex2f(xpBarX + xpFilledWidth, xpBarY);
        glVertex2f(xpBarX + xpFilledWidth, xpBarY + xpBarHeight);
        glVertex2f(xpBarX, xpBarY + xpBarHeight);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();

    float baseWeaponIconSize = 200.0f * scaleX; // Base size for DEFAULT and LASER
    float weaponStartX = dim.x - (150.0f * scaleX); // Right-aligned starting X
    float weaponStartY = dim.y - (300.0f * scaleY); // Starting Y (top of stack)

    int slot = 0;

    const float rocketPositions[] = { dim.y - (200.0f * scaleY), dim.y - (355.0f * scaleY), dim.y - (470.0f * scaleY), dim.y - (585.0f * scaleY)};
    const float laserPositions[] = { dim.y - (200.0f * scaleY), dim.y - (410.0f * scaleY), dim.y - (520.0f * scaleY), dim.y - (630.0f * scaleY)};
    const float flakPositions[] = { dim.y - (200.0f * scaleY), dim.y - (345.0f * scaleY), dim.y - (457.0f * scaleY), dim.y - (568.0f * scaleY)};

    for (const auto& weapon : player->weapons)
    {
        if (!weapon.isActive) continue;

        shared_ptr<_textureLoader> weaponTex;
        float iconSize = baseWeaponIconSize; // Default size
        float currentY = weaponStartY;
        float currentX = weaponStartX;
        float textOffsetX = 0.0f;
        float textOffsetY = 0.0f;

        switch (weapon.type)
        {
            case DEFAULT:
                weaponTex = player->bulletTextureLoader;
                currentY = weaponStartY;
                currentX = weaponStartX;
                textOffsetX = -25.0f * scaleX;
                textOffsetY = -5.0f * scaleY;
                break;
            case ROCKET:
                weaponTex = player->rocketTex;
                iconSize = baseWeaponIconSize / 2.0f;
                currentY = rocketPositions[slot];
                currentX += 50.0f*scaleX;
                textOffsetX = -75.0f * scaleX;
                textOffsetY = -10.0f;
                break;
            case LASER:
                weaponTex = player->laserTex;
                currentY = laserPositions[slot];
                currentX -= 75.0f*scaleX;
                textOffsetX = 50.0f * scaleX;
                textOffsetY = -5.0f * scaleY;
                break;
            case FLAK:
                weaponTex = player->flakTex;
                iconSize = baseWeaponIconSize / 3.0f;
                currentY = flakPositions[slot];
                 currentX += 60.0f*scaleX;
                textOffsetX = -85.0f * scaleX;
                textOffsetY = -5.0f;
                break;
            default:
                continue;
        }

        float weaponX = currentX;
        float weaponY = currentY;

        if (weaponTex)
        {
            glPushMatrix();
            weaponTex->textureBinder();
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(weaponX, weaponY);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(weaponX + iconSize, weaponY);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(weaponX + iconSize, weaponY + iconSize);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(weaponX, weaponY + iconSize);
            glEnd();
            glPopMatrix();
        }

        // Render level text to the left of the icon
        string levelText = "LV" + std::to_string(weapon.level);
        float textX = weaponX + textOffsetX - (12.0f * scaleX); // Adjust left of icon
        float textY = weaponY + (iconSize / 2.0f) + textOffsetY; // Center vertically
        renderText(levelText, textX, textY, scaleY * 0.8f, 0.0f);

        slot++;
    }

    // Upgrade Icons (left side, stacked vertically)
    float baseUpgradeIconSize = 150.0f * scaleX; // Base size (adjust as needed)
    float upgradeStartX = 0; // 50px from left edge
    float upgradeStartY = dim.y - (150.0f * scaleY); // Start near top
    int upgradeSlot = 0;

    const float damagePositions[] = { dim.y - (280.0f * scaleY), dim.y - (395.0f * scaleY), dim.y - (505.0f * scaleY), dim.y - (620.0f * scaleY)};
    const float fireRatePositions[] = { dim.y - (260.0f * scaleY), dim.y - (375.0f * scaleY), dim.y - (485.0f * scaleY), dim.y - (595.0f * scaleY)};
    const float aoeSizePositions[] = { dim.y - (250.0f * scaleY), dim.y - (365.0f * scaleY), dim.y - (475.0f * scaleY), dim.y - (585.0f * scaleY)};
    const float speedPositions[] = { dim.y - (290.0f * scaleY), dim.y - (400.0f * scaleY), dim.y - (510.0f * scaleY), dim.y - (620.0f * scaleY)};
    const float healthPositions[] = { dim.y - (255.0f * scaleY), dim.y - (365.0f * scaleY), dim.y - (480.0f * scaleY), dim.y - (590.0f * scaleY)};

    float textOffsetX = 0;
    float textOffsetY = 0;

    // Map upgrade names to textures
    std::map<std::string, _textureLoader*> upgradeTextures = {
        {"Damage", damageIconTexture},
        {"FireRate", fireRateIconTexture},
        {"AoeSize", aoeSizeIconTexture},
        {"Speed", speedIconTexture},
        {"Health", healthIconTexture}
    };

    // Render active upgrade icons
    for (const auto& upgrade : activeUpgrades)
    {
        auto texIt = upgradeTextures.find(upgrade.name);
        if (texIt == upgradeTextures.end()) continue; // Skip if no texture

        _textureLoader* tex = texIt->second;
        float iconSize = baseUpgradeIconSize; // Default size

        float currentY = upgradeStartY;
        float currentX = upgradeStartX;

        // Adjust size per upgrade type (example values, tweak based on PNG sizes)
        if (upgrade.name == "Damage")
        {
            iconSize = baseUpgradeIconSize * 1.0f;
            currentX = -30.0f * scaleX;
            currentY = damagePositions[upgradeSlot];
            textOffsetX = 0;
            textOffsetY = 0;
        }
        else if (upgrade.name == "FireRate")
        {
            iconSize = baseUpgradeIconSize * 0.8f;
            currentX = -5.0f * scaleX;
            currentY = fireRatePositions[upgradeSlot];
            textOffsetX = 5.0f;
            textOffsetY = -3.0f;
        }
        else if (upgrade.name == "AoeSize")
        {
            iconSize = baseUpgradeIconSize * 0.7f;
            currentX = -5.0f * scaleX;
            currentY = aoeSizePositions[upgradeSlot];
            textOffsetX = 25.0f;
            textOffsetY = -3.0;
        }
        else if (upgrade.name == "Speed")
        {
            iconSize = baseUpgradeIconSize * 1.25f;
            currentX = -50.0f * scaleX;
            currentY = speedPositions[upgradeSlot];
            textOffsetX = -25.0f;
            textOffsetY = -10.0f;
        }
        else if (upgrade.name == "Health")
        {
            iconSize = baseUpgradeIconSize * 0.8f;
            currentX = -12.0f * scaleX;
            currentY = healthPositions[upgradeSlot];
            textOffsetX = 12.0f;
            textOffsetY = -10.0f;
        }

        glPushMatrix();
        if (tex) {
            tex->textureBinder();
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(currentX, currentY);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(currentX + iconSize, currentY);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(currentX + iconSize, currentY + iconSize);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(currentX, currentY + iconSize);
            glEnd();
        }
        glPopMatrix();

        // Optionally, render level text next to the icon
        string levelText = "LV" + std::to_string(upgrade.level);
        float textX = textOffsetX + currentX + iconSize + (12.0f * scaleX); // 10px right of icon
        float textY = textOffsetY + currentY + (iconSize / 2.0f); // Center vertically
        renderText(levelText, textX, textY, scaleY * 0.8f, 0.0f);

        upgradeSlot++;
        if (upgradeSlot >= 5) break; // Limit to 5 slots
    }

    // Upgrade Menu (unchanged, but ensure text is visible)
    if (upgradeMenuActive) {
        glPushMatrix();
        glDisable(GL_TEXTURE_2D);
        glColor4f(0.0f, 0.0f, 0.0f, 0.7f); // Semi-transparent background
        glBegin(GL_QUADS);
            glVertex2f(0, 0);
            glVertex2f(dim.x, 0);
            glVertex2f(dim.x, dim.y);
            glVertex2f(0, dim.y);
        glEnd();

        float boxWidth = 300.0f * scaleX;
        float boxHeight = 500.0f * scaleY;
        float spacing = 50.0f * scaleX;
        float totalWidth = (3 * boxWidth) + (2 * spacing);
        float startX = (dim.x - totalWidth) / 2.0f;
        float startY = dim.y / 2.0f - (boxHeight / 2.0f);

        for (size_t i = 0; i < currentUpgradeOptions.size() && i < 3; ++i) {
            float x = startX + i * (boxWidth + spacing);
            glColor3f(0.3f, 0.3f, 0.3f); // Gray box
            glBegin(GL_QUADS);
                glVertex2f(x, startY);
                glVertex2f(x + boxWidth, startY);
                glVertex2f(x + boxWidth, startY + boxHeight);
                glVertex2f(x, startY + boxHeight);
            glEnd();
            string text = to_string(i + 1) + ": " + currentUpgradeOptions[i].displayText;
            float textX = x + (20.0f * scaleX);
            float textY = startY + (boxHeight / 2.0f) + (5.0f * scaleY);
            glColor3f(1.0f, 1.0f, 1.0f);
            glRasterPos2f(textX, textY);
            for (char c : text) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        }
        glEnable(GL_TEXTURE_2D);
        glPopMatrix();
    }

    // Draw HUD Texture (full screen)
    float hudWidth = dim.x;
    float hudHeight = dim.y;
    glPushMatrix();
    if (hudTexture) {
        hudTexture->textureBinder();
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f); // Bottom-left
        glTexCoord2f(1.0f, 1.0f); glVertex2f(hudWidth, 0.0f); // Bottom-right
        glTexCoord2f(1.0f, 0.0f); glVertex2f(hudWidth, hudHeight); // Top-right
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, hudHeight); // Top-left
    glEnd();
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void _scene::showUpgradeMenu() {
    // Count current non-weapon upgrades and weapons
    int nonWeaponUpgradeCount = 0;
    int weaponCount = 0;
    for (const auto& upgrade : activeUpgrades) {
        if (upgrade.isWeapon) {
            weaponCount++;
        } else {
            nonWeaponUpgradeCount++;
        }
    }

    // Filter available upgrades
    vector<string> validUpgrades;
    for (const auto& upgrade : availableUpgrades) {
        bool isWeapon = (upgrade.find("Weapon_") == 0);
        string baseName = isWeapon ? upgrade.substr(7) : upgrade; // e.g., "Laser" or "Damage"

        // Find current level of this upgrade/weapon
        auto it = std::find_if(activeUpgrades.begin(), activeUpgrades.end(),
            [&upgrade](const ActiveUpgrade& up) { return up.name == upgrade; });
        int currentLevel = (it != activeUpgrades.end()) ? it->level : 0;

        // Skip if already at max level
        if (currentLevel >= MAX_UPGRADE_LEVEL) {
            continue;
        }

        // Non-weapon upgrades: include if under the limit or already active
        if (!isWeapon) {
            if (nonWeaponUpgradeCount < MAX_NON_WEAPON_UPGRADES || currentLevel > 0) {
                validUpgrades.push_back(upgrade);
            }
        }
        // Weapon upgrades: include if under the limit or already active
        else {
            // Allow Weapon_Default to be leveled up if active and below max level
            if (baseName == "Default" && currentLevel > 0 && currentLevel < MAX_UPGRADE_LEVEL) {
                validUpgrades.push_back(upgrade);
                continue;
            }
            // For other weapons, check if we can add a new weapon or level up an existing one
            if (weaponCount < MAX_WEAPONS || currentLevel > 0) {
                validUpgrades.push_back(upgrade);
            }
        }
    }

    // If no valid upgrades, skip the menu and resume gameplay
    if (validUpgrades.empty()) {
        upgradeMenuActive = false;
        isPaused = false;
        currentUpgradeOptions.clear();
        return;
    }

    // Select up to 3 random valid upgrades
    upgradeMenuActive = true;
    isPaused = true;
    currentUpgradeOptions.clear();
    random_shuffle(validUpgrades.begin(), validUpgrades.end());
    int optionsToSelect = min(3, (int)validUpgrades.size());

    for (int i = 0; i < optionsToSelect; ++i) {
        UpgradeOption option;
        option.name = validUpgrades[i];
        option.isWeapon = (option.name.find("Weapon_") == 0);

        if (option.isWeapon) {
            string weaponName = option.name.substr(7);
            if (weaponName == "Default") option.weaponType = DEFAULT;
            else if (weaponName == "Rocket") option.weaponType = ROCKET;
            else if (weaponName == "Laser") option.weaponType = LASER;
            else if (weaponName == "Flak") option.weaponType = FLAK;

            auto it = std::find_if(player->weapons.begin(), player->weapons.end(),
                [&](const Weapon& w) { return w.type == option.weaponType; });
            option.currentLevel = (it != player->weapons.end()) ? it->level : 0;
        } else {
            if (option.name == "Damage") option.currentLevel = (int)((player->damageMultiplier - 1.0f) / 0.1f);
            else if (option.name == "Speed") option.currentLevel = (int)((player->speedMultiplier - 1.0f) / 0.1f);
            else if (option.name == "Health") option.currentLevel = (int)((player->healthMultiplier - 1.0f) / 0.1f);
            else if (option.name == "FireRate") option.currentLevel = (int)((1.0f - player->fireRateMultiplier) / 0.1f);
            else if (option.name == "AoeSize") option.currentLevel = (int)((player->aoeSizeMultiplier - 1.0f) / 0.1f);
        }

        // Format display text
        option.displayText = option.name + ": Level " + std::to_string(option.currentLevel + 1);
        if (!option.isWeapon) {
            if (option.name == "Damage") option.displayText += " - 10% increased damage";
            if (option.name == "Speed") option.displayText += " - 10% faster movement";
            if (option.name == "Health") option.displayText += " - 10% more max HP";
            if (option.name == "FireRate") option.displayText += " - 10% faster fire rate";
            if (option.name == "AoeSize") option.displayText += " - 10% increased AoE";
        }

        currentUpgradeOptions.push_back(option);
    }
}

void _scene::selectUpgrade(int choice) {
    if (choice >= 0 && choice < (int)currentUpgradeOptions.size()) {
        UpgradeOption& selected = currentUpgradeOptions[choice];

        // Apply the upgrade
        if (selected.isWeapon) {
            player->applyWeaponUpgrade(selected.weaponType);
        } else {
            player->applyUpgrade(selected.name);
        }

        // Update activeUpgrades
        auto it = std::find_if(activeUpgrades.begin(), activeUpgrades.end(),
            [&selected](const ActiveUpgrade& up) { return up.name == selected.name; });
        if (it != activeUpgrades.end()) {
            if (it->level < MAX_UPGRADE_LEVEL) {
                it->level = selected.currentLevel + 1;
            }
        } else {
            activeUpgrades.push_back({selected.name, selected.currentLevel + 1, selected.isWeapon});
        }
    }
    upgradeMenuActive = false;
    isPaused = false;
    currentUpgradeOptions.clear();
}

void _scene::reSize(GLint width, GLint height){
    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

    GLfloat aspectRatio = (GLfloat)width/(GLfloat)height;  // Resize Window Ratio
    glViewport(0,0,width,height);                           // Viewport
    glMatrixMode(GL_PROJECTION);                            // Perspective Projection Settings
    glLoadIdentity();                                       // Identity Matrix
    gluPerspective(45,aspectRatio,0.1,100.0);               // Projection
    glMatrixMode(GL_MODELVIEW);                             // Camera and Model Settings
    glLoadIdentity();                                       // Identity Matrix

}

void _scene::processKeyboardInput() {
    updateDeltaTime();

    // Update worldMousePos
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(GetActiveWindow(), &mousePos);
    worldMousePos.x = (mousePos.x - dim.x / 2) / (float)(dim.x / 2);
    worldMousePos.y = (dim.y / 2 - mousePos.y) / (float)(dim.y / 2);
    worldMousePos.z = 0.0f; // 2D game, z typically 0

    if (upgradeMenuActive) {
        // Handle upgrade selection (keys 1�3)
        if (GetAsyncKeyState('1') & 0x8000) {
            selectUpgrade(0);
        }
        else if (GetAsyncKeyState('2') & 0x8000) {
            selectUpgrade(1);
        }
        else if (GetAsyncKeyState('3') & 0x8000) {
            selectUpgrade(2);
        }
        return; // Skip other inputs while menu is active
    }

    if (!isPaused && !gameOver) {
        input->updateMouseRotation(player, mousePos.x, mousePos.y, dim.x, dim.y);
    }

    // Toggle debug mode with 'F' key
    static bool lastState = false;
    if (GetAsyncKeyState('F') & 0x8000) {
        bool currentState = true;
        if (!lastState && currentState) {
            debugMode = !debugMode;
        }
        lastState = currentState;
    } else {
        lastState = false;
    }

    // Toggle pause with 'P' key
    static bool lastPauseState = false;
    if (GetAsyncKeyState('P') & 0x8000) {
        bool currentState = true;
        if (!lastPauseState && currentState) {
            isPaused = !isPaused;
        }
        lastPauseState = currentState;
    } else {
        lastPauseState = false;
    }

    if (!isPaused && !gameOver) {
        input->keyPressed(player, sounds, deltaTime);
        input->keyUp(player, sounds);
    }
}

void _scene::updateEnemySpawning(){
    if (elapsedTime >= 1800.0f || gameOver) // Stop spawning after 30 minutes
        return;

    // Dynamic spawn interval for regular enemies
    float timeFactor = elapsedTime / 600.0f; // Normalize time over 10 minutes
    spawnInterval = 2.0f * exp(-timeFactor) + 0.1f;
    spawnInterval = fmax(0.1f, spawnInterval);

    // Dynamic enemy cap
    int maxEnemies = 200 + static_cast<int>((elapsedTime / 1200.0f) * 450);
    maxEnemies = min(maxEnemies, 500);

    // Regular enemy spawning
    if (elapsedTime - lastSpawnTime >= spawnInterval)
    {
        if (enemies.size() < static_cast<size_t>(maxEnemies))
        {
            int batchSize = 10 + static_cast<int>(timeFactor * 4);
            batchSize = min(batchSize, 30);

            for (int i = 0; i < batchSize; i++)
            {
                vec3 randPos;
                float angle = (rand() % 360) * (M_PI / 180.0f);
                float distance = 15.0f + (rand() % 11) * 1.0f;
                randPos.x = player->playerPosition.x + cos(angle) * distance;
                randPos.y = player->playerPosition.y + sin(angle) * distance;
                randPos.z = 48.0f;

                bool reused = false;
                for (auto& enemy : enemies)
                {
                    if (!enemy.isAlive)
                    {
                        enemy.placeEnemy(randPos);
                        enemy.isAlive = true;
                        enemy.initEnemy("images/swarmbot.png", 20.0f, {0.6f, 0.6f, 1.0f}, 2.0f); // Regular enemy stats
                        enemy.scale = {1.0f, 1.0f};
                        enemy.playerPosition = player->playerPosition; // Set initial position
                        enemy.currentHp = enemy.maxHp;
                        reused = true;
                        break;
                    }
                }
                if (!reused && enemies.size() < static_cast<size_t>(maxEnemies))
                {
                    _enemy newEnemy; // Regular enemy stats
                    newEnemy.placeEnemy(randPos);
                    newEnemy.isAlive = true;
                    newEnemy.initEnemy("images/swarmbot.png", 20.0f, {0.6f, 0.6f, 1.0f}, 2.0f);
                    newEnemy.scale = {1.0f, 1.0f};
                    newEnemy.playerPosition = player->playerPosition; // Set initial position
                    enemies.push_back(newEnemy);
                }
            }

            lastSpawnTime = elapsedTime;
        }
    }

    // Bug swarm spawning
    if (elapsedTime - lastBugSpawnTime >= bugSpawnInterval)
    {
        spawnBugSwarm();
        lastBugSpawnTime = elapsedTime;
    }

}

void _scene::spawnBugSwarm(){
    int maxEnemies = 100 + static_cast<int>((elapsedTime / 1200.0f) * 450);
    maxEnemies = min(maxEnemies, 500);

    if (enemies.size() >= static_cast<size_t>(maxEnemies))
        return;

    // Choose a central spawn point
    float angle = (rand() % 360) * (M_PI / 180.0f);
    float distance = 15.0f + (rand() % 11) * 1.0f; // 15�25 units from player
    vec3 centerPos;
    centerPos.x = player->playerPosition.x + cos(angle) * distance;
    centerPos.y = player->playerPosition.y + sin(angle) * distance;
    centerPos.z = 48.0f;

    // Spawn 20�50 bugs in a small radius (clumped)
    int batchSize = 50 + (rand() % 51); // Random 20�50 bugs
    float swarmRadius = 2.0f; // Bugs spawn within 2 units of center

    for (int i = 0; i < batchSize; i++)
    {
        // Random offset within swarm radius
        float offsetAngle = (rand() % 360) * (M_PI / 180.0f);
        float offsetDist = (rand() % 100) / 100.0f * swarmRadius;
        vec3 randPos;
        randPos.x = centerPos.x + cos(offsetAngle) * offsetDist;
        randPos.y = centerPos.y + sin(offsetAngle) * offsetDist;
        randPos.z = centerPos.z;

        // Reuse dead enemies or add new ones
        bool reused = false;
        for (auto& enemy : enemies)
        {
            if (!enemy.isAlive)
            {
                enemy.placeEnemy(randPos);
                enemy.isAlive = true;
                enemy.initEnemy("images/smallbug.png", 10.0f, {0.3f, 0.3f, 1.0f}, 4.0); // Bug stats
                enemy.scale = {1.0f, 1.0f}; // Smaller size
                enemy.playerPosition = player->playerPosition; // Set initial position
                enemy.currentHp = enemy.maxHp;
                reused = true;
                break;
            }
        }
        if (!reused && enemies.size() < static_cast<size_t>(maxEnemies))
        {
            _enemy newEnemy; // Bug stats
            newEnemy.placeEnemy(randPos);
            newEnemy.isAlive = true;
            newEnemy.initEnemy("images/smallbug.png", 5.0f, {0.3f, 0.3f, 1.0f}, 4.0);
            newEnemy.scale = {1.0f, 1.0f}; // Smaller size
            newEnemy.playerPosition = player->playerPosition; // Set initial position
            enemies.push_back(newEnemy);
        }
    }
}

void _scene::updateDeltaTime(){
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);  // Pass pointer to currentTime

    static bool wasPausedLastFrame = false;  // Declare at function scope
    if (!isPaused)
    {
        if (wasPausedLastFrame)
        {
            // First frame after unpause: reset lastTime but don�t calculate deltaTime yet
            lastTime = currentTime;
            deltaTime = 0.0f;
            wasPausedLastFrame = false;
        }
        else
        {
            deltaTime = (float)(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
            lastTime = currentTime;
            elapsedTime += deltaTime;
        }
    }
    else
    {
        deltaTime = 0.0f;
        wasPausedLastFrame = true;  // Track that we�re paused
    }
}

void _scene::renderText(string text, float x, float y, float scale, float spacing) {
if (!fontTexture || fontTexture->tex == 0) {
        std::cerr << "Font texture is null or invalid!" << std::endl;
        return;
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    fontTexture->textureBinder();

    glColor3f(1.0f, 1.0f, 1.0f);

    float currentX = x;
    for (char c : text) {
        int ascii = static_cast<unsigned char>(c);
        if (bitmapFont.find(ascii) == bitmapFont.end()) {
            std::cerr << "Character '" << c << "' (ASCII " << ascii << ") not found in font!" << std::endl;
            continue;
        }

        FontChar& fc = bitmapFont[ascii];

        float w = fc.width * scale;
        float h = fc.height * scale;
        float xoff = fc.xoffset * scale;
        float yoff = fc.yoffset * scale;

        float u0 = fc.u0;
        float v0 = fc.v0; // Try without flipping for testing
        float u1 = fc.u1;
        float v1 = fc.v1;

        glBegin(GL_QUADS);
            glTexCoord2f(u0, v1); glVertex2f(currentX + xoff,     y + yoff);
            glTexCoord2f(u1, v1); glVertex2f(currentX + xoff + w, y + yoff);
            glTexCoord2f(u1, v0); glVertex2f(currentX + xoff + w, y + yoff + h);
            glTexCoord2f(u0, v0); glVertex2f(currentX + xoff,     y + yoff + h);
        glEnd();

        currentX += fc.xadvance * scale + spacing;
    }

}

void _scene::loadFontData(const string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open font file: " << path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("char id=") == std::string::npos) continue;

        FontChar fc;

        sscanf(line.c_str(),
            "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d",
            &fc.id, &fc.x, &fc.y, &fc.width, &fc.height,
            &fc.xoffset, &fc.yoffset, &fc.xadvance);

        float texWidth = 256.0f;
        float texHeight = 256.0f;

        fc.u0 = fc.x / texWidth;
        fc.v0 = fc.y / texHeight;
        fc.u1 = (fc.x + fc.width) / texWidth;
        fc.v1 = (fc.y + fc.height) / texHeight;

        bitmapFont[fc.id] = fc;
    }

}
