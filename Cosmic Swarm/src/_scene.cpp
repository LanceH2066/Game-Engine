#include "_scene.h"

_scene::_scene(){
    input = new _inputs();
    prlx1 = new _parallax();
    player = new _player();
    collision = new _collision();
    sounds = new _sounds();
    xpOrbTexture = new _textureLoader();
    if (!input || !prlx1 || !player || !collision || !sounds || !xpOrbTexture) {
        std::cout << "Failed to allocate scene objects!" << std::endl;
    }

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
                            enemies[i].takeDamage(bullet.weapon.damage * deltaTime, xpOrbs, xpOrbTexture);
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
                                    enemies[i].takeDamage(bullet.weapon.damage, xpOrbs, xpOrbTexture);
                                }
                                bullet.hitEnemies.push_back(i);
                                if (bullet.weapon.type != FLAK)
                                {
                                    if (bullet.weapon.type == ROCKET)
                                    {
                                        if(!bullet.hasExploded)
                                        {
                                            bullet.explode(enemies, xpOrbs, xpOrbTexture);
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
        orb.update(deltaTime, player->playerPosition, xpPickupRange);
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

    // Setup orthographic projection for 2D HUD
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, dim.x, 0, dim.y);  // Match window dimensions

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Draw XP Bar in screen space
    player->drawXPBar();
    player->drawHealthBar();

    if (upgradeMenuActive)
    {
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);

    // Draw semi-transparent background
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(dim.x, 0);
        glVertex2f(dim.x, dim.y);
        glVertex2f(0, dim.y);
    glEnd();

    // Draw upgrade options
    float boxWidth = 300.0f;
    float boxHeight = 100.0f;
    float startY = dim.y / 2 - (currentUpgradeOptions.size() * boxHeight) / 2;
    float startX = dim.x / 2 - boxWidth / 2;

    for (size_t i = 0; i < currentUpgradeOptions.size(); ++i) {
        float y = startY + i * (boxHeight + 10.0f);

        // Draw box
        glColor3f(0.3f, 0.3f, 0.3f);
        glBegin(GL_QUADS);
            glVertex2f(startX, y);
            glVertex2f(startX + boxWidth, y);
            glVertex2f(startX + boxWidth, y + boxHeight);
            glVertex2f(startX, y + boxHeight);
        glEnd();

        // Draw text
        string text = to_string(i + 1) + ": " + currentUpgradeOptions[i];
        float textX = startX + 20.0f;
        float textY = y + boxHeight / 2 + 5.0f;
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(textX, textY);
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  // Return to 3D mode

}

void _scene::showUpgradeMenu() {
    upgradeMenuActive = true;
    isPaused = true; // Pause game during upgrade selection

    // Select 3 random upgrades
    currentUpgradeOptions.clear();
    vector<string> tempUpgrades = availableUpgrades;
    random_shuffle(tempUpgrades.begin(), tempUpgrades.end());
    for (int i = 0; i < min(3, (int)tempUpgrades.size()); ++i) {
        currentUpgradeOptions.push_back(tempUpgrades[i]);
    }
}

void _scene::selectUpgrade(int choice) {
    if (choice >= 0 && choice < (int)currentUpgradeOptions.size()) {
        player->applyUpgrade(currentUpgradeOptions[choice]);
    }
    upgradeMenuActive = false;
    isPaused = false; // Resume game
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
        // Handle upgrade selection (keys 1–3)
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
    float distance = 15.0f + (rand() % 11) * 1.0f; // 15–25 units from player
    vec3 centerPos;
    centerPos.x = player->playerPosition.x + cos(angle) * distance;
    centerPos.y = player->playerPosition.y + sin(angle) * distance;
    centerPos.z = 48.0f;

    // Spawn 20–50 bugs in a small radius (clumped)
    int batchSize = 50 + (rand() % 51); // Random 20–50 bugs
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
            // First frame after unpause: reset lastTime but don’t calculate deltaTime yet
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
        wasPausedLastFrame = true;  // Track that we’re paused
    }
}
