#include "_scene.h"

_inputs *input = new _inputs();
_parallax *prlx1 = new _parallax();
_player *player = new _player();
_collision *collision = new _collision();
_sounds *sounds = new _sounds();
_textureLoader* xpOrbTexture = new _textureLoader();

_scene::_scene()
{
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);
    deltaTime = 0.0f;
    enemies.clear();
}

_scene::~_scene()
{
    delete input;
    delete prlx1;
    delete player;
    delete collision;
    delete sounds;
}

GLint _scene::initGL()
{
    srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 0; i < 20; ++i) rand();
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
    xpOrbTexture->loadTexture("images/scrap.png");

    // Convert screen pixels to world space (assumes orthographic units)
    float worldUnitsPerPixel = 10.0f / dim.x; // Adjust based on projection settings
    float screenWidthUnits = dim.x * worldUnitsPerPixel;

    float minDistance = screenWidthUnits * 1.2f; // Slightly offscreen
    float maxDistance = screenWidthUnits * 1.5f; // More variation

    // INITIALIZE SOUNDS & START MUSIC
    sounds->playMusic();

    return true;
}

void _scene::drawScene()
{
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
                if (!isPaused)
                {
                    bullet.update(deltaTime);
                }
                bullet.drawBullet();

                vec3 bulletMin = bullet.getCollisionBoxMin();
                vec3 bulletMax = bullet.getCollisionBoxMax();

                if (!isPaused)
                {
                    for (size_t i = 0; i < enemies.size(); i++)
                    {
                        // Check if this enemy has already been hit by this bullet
                        if (find(bullet.hitEnemies.begin(), bullet.hitEnemies.end(), i) == bullet.hitEnemies.end())
                        {
                            if (collision->isOBBCollision(bullet, enemies[i]))     //bullet hits enemy ---------------------------- xp orb code
                            {
                                bool wasAlive = enemies[i].isAlive;

                                enemies[i].takeDamage(bullet.damage);
                                bullet.hitEnemies.push_back(i);  // Record this enemy as hit

                                if (wasAlive && !enemies[i].isAlive)
                                {
                                    _xpOrb orb;
                                    orb.xpTextureLoader = xpOrbTexture;        // Assign shared texture loader
                                    orb.placeOrb(enemies[i].position);
                                    orb.initOrb("images/scrap.png");   // This now uses the correct texture loader
                                    xpOrbs.push_back(orb);
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
        orb.drawOrb();
    }

    //check for XP orb pickups
    for (auto& orb : xpOrbs)
    {
        if (orb.isActive && collision->isOBBCollision(*player, orb))
        {
            orb.isActive = false;
            player->gainXP(1);  // player gain 1 XP
        }
    }

    //remove collected orbs
    xpOrbs.erase(
    remove_if(xpOrbs.begin(), xpOrbs.end(), [](const _xpOrb& o) { return !o.isActive; }),
    xpOrbs.end()
    );
}

void _scene::reSize(GLint width, GLint height)
{
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

void _scene::processKeyboardInput()
{
    updateDeltaTime();

    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(GetActiveWindow(), &mousePos);

    vec3 worldMousePos;
    worldMousePos.x = (mousePos.x - dim.x / 2) / (float)(dim.x / 2);
    worldMousePos.y = (dim.y / 2 - mousePos.y) / (float)(dim.y / 2);

    if (!isPaused && !gameOver)
    {
        input->updateMouseRotation(player, mousePos.x, mousePos.y, dim.x, dim.y);
        player->bulletTimer.update(deltaTime);  // Update timer with deltaTime
    }

    if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && !isPaused && !gameOver)
    {
        player->shoot(worldMousePos, sounds);
    }

    // Toggle debug mode with 'F' key
    static bool lastState = false;
    if (GetAsyncKeyState('F') & 0x8000)
    {
        bool currentState = true;
        if (!lastState && currentState)
        {
            debugMode = !debugMode;
        }
        lastState = currentState;
    }
    else
    {
        lastState = false;
    }

    // Toggle pause with 'P' key
    static bool lastPauseState = false;
    if (GetAsyncKeyState('P') & 0x8000)
    {
        bool currentState = true;
        if (!lastPauseState && currentState)
        {
            isPaused = !isPaused;
            if (isPaused)
            {
                player->bulletTimer.pause();
            }
            else
            {
                player->bulletTimer.unPause();
            }
        }
        lastPauseState = currentState;
    }
    else
    {
        lastPauseState = false;
    }

    if (!isPaused && !gameOver)
    {
        input->keyPressed(player, sounds, deltaTime);
        input->keyUp(player, sounds);
    }
}

void _scene::updateEnemySpawning()
{
    if (elapsedTime >= 1800.0f || gameOver)  // Stop spawning after 30 minutes or on game over (adjust as needed)
        return;

    // Dynamic spawn interval: decreases exponentially from 2.0 to 0.1 over time
    float timeFactor = elapsedTime / 600.0f;  // Normalize time over 10 minutes
    spawnInterval = 2.0f * exp(-timeFactor) + 0.1f;  // Exponential decay + minimum interval
    spawnInterval = fmax(0.1f, spawnInterval);  // Clamp to minimum 0.1 seconds

    // Dynamic enemy cap: scales from 50 to 500 over 20 minutes
    int maxEnemies = 50 + static_cast<int>((elapsedTime / 1200.0f) * 450);  // Linear increase
    maxEnemies = min(maxEnemies, 500);  // Hard cap at 500 for performance

    if (elapsedTime - lastSpawnTime >= spawnInterval)
    {
        if (enemies.size() < static_cast<size_t>(maxEnemies))
        {
            // Batch spawning: spawn 1–5 enemies at a time, scaling with time
            int batchSize = 10 + static_cast<int>(timeFactor * 4);  // 1 at start, up to 5 later
            batchSize = min(batchSize, 30);  // Cap batch size

            for (int i = 0; i < batchSize; i++)
            {
                vec3 randPos;
                float angle = (rand() % 360) * (M_PI / 180.0f);
                float distance = 15.0f + (rand() % 11) * 1.0f;  // Random distance 15–25 units
                randPos.x = player->playerPosition.x + cos(angle) * distance;
                randPos.y = player->playerPosition.y + sin(angle) * distance;
                randPos.z = 48.0f;

                // Reuse dead enemies or add new ones
                bool reused = false;
                for (auto& enemy : enemies)
                {
                    if (!enemy.isAlive)
                    {
                        enemy.placeEnemy(randPos);
                        enemy.isAlive = true;
                        enemy.initEnemy("images/swarmbot.png");
                        enemy.setPlayerReference(player);
                        enemy.currentHp = enemy.maxHp;  // Reset health
                        reused = true;
                        break;
                    }
                }
                if (!reused && enemies.size() < static_cast<size_t>(maxEnemies))
                {
                    _enemy newEnemy;
                    newEnemy.placeEnemy(randPos);
                    newEnemy.isAlive = true;
                    newEnemy.initEnemy("images/swarmbot.png");
                    newEnemy.setPlayerReference(player);
                    enemies.push_back(newEnemy);
                }
            }

            lastSpawnTime = elapsedTime;
        }
    }
}

void _scene::updateDeltaTime()
{
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
