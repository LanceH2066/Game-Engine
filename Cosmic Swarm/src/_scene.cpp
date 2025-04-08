#include "_scene.h"

_inputs *input = new _inputs();
_parallax *prlx1 = new _parallax();
_player *player = new _player();
_enemy enemies[50];
_collision *collision = new _collision();
_sounds *sounds = new _sounds();

_scene::_scene()
{
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);
    deltaTime = 0.0f;
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

    // Draw background (always render, even when paused)
    glPushMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        prlx1->drawBackground(dim.x, dim.y, player->playerPosition);
    glPopMatrix();

    // Draw player (always render)
    glPushMatrix();
        player->drawPlayer();
    glPopMatrix();

    // Bullets: update only if not paused, draw always
    glPushMatrix();
        if (!isPaused) {
            player->bullets.erase(
                remove_if(player->bullets.begin(), player->bullets.end(), [](const _Bullet& b) { return !b.isAlive; }),
                player->bullets.end()
            );
        }
        for (auto& bullet : player->bullets)
        {
            if (bullet.isAlive)
            {
                // BULLET MOVEMENT
                if (!isPaused)
                {
                    bullet.update(deltaTime);
                }

                bullet.drawBullet();

                vec3 bulletMin = bullet.getCollisionBoxMin();
                vec3 bulletMax = bullet.getCollisionBoxMax();

                // BULLET COLLISION
                if (!isPaused)
                {
                    for (int i = 0; i < maxEnemies; i++)
                    {
                        if (enemies[i].isAlive)
                        {
                            vec3 enemyMin = enemies[i].getCollisionBoxMin();
                            vec3 enemyMax = enemies[i].getCollisionBoxMax();
                            if (collision->isOBBCollision(bulletMin, bulletMax, enemyMin, enemyMax))
                            {
                                bullet.isAlive = false;
                                enemies[i].takeDamage(bullet.damage);
                            }
                        }
                    }
                }

                // DRAW HITBOXES ON BULLETS
                if (debugMode)
                {
                    glPushMatrix();
                    glDisable(GL_TEXTURE_2D);  // Disable texturing for hitbox
                        glTranslatef(bullet.position.x, bullet.position.y, bullet.position.z + 0.1f);
                        glColor3f(0.0f, 1.0f, 0.0f);
                        glBegin(GL_LINE_LOOP);
                            glVertex3f(bulletMin.x - bullet.position.x, bulletMin.y - bullet.position.y, 0);
                            glVertex3f(bulletMax.x - bullet.position.x, bulletMin.y - bullet.position.y, 0);
                            glVertex3f(bulletMax.x - bullet.position.x, bulletMax.y - bullet.position.y, 0);
                            glVertex3f(bulletMin.x - bullet.position.x, bulletMax.y - bullet.position.y, 0);
                        glEnd();
                        glColor3f(1.0f, 1.0f, 1.0f);
                        glEnable(GL_TEXTURE_2D);  // Re-enable texturing
                    glPopMatrix();
                }
            }
        }
    glPopMatrix();

    // Player actions and enemy updates only if not paused
    if (!isPaused)
    {
        player->playerActions(deltaTime);
        updateEnemySpawning();
    }

    // Enemies: update only if not paused, draw always
    glPushMatrix();
        for (int i = 0; i < 20; i++)
        {
            if (!isPaused)
            {
                for (int j = i + 1; j < 20; j++)
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

                if (collision->isRadialCollision(enemies[i].position, player->playerPosition, 0.5, 0.5, 0.02))
                {
                    enemies[i].actionTrigger = enemies[i].ATTACK;
                }
                else
                {
                    enemies[i].actionTrigger = enemies[i].PURSUIT;
                }

                enemies[i].enemyActions(deltaTime);
            }

            enemies[i].drawEnemy(enemies[i].enemyTextureLoader->tex);

            if (debugMode && enemies[i].isAlive)
            {
                glPushMatrix();
                    glDisable(GL_TEXTURE_2D);
                    glTranslatef(enemies[i].position.x, enemies[i].position.y, enemies[i].position.z + 0.1f);
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

    if (!isPaused)
    {
        input->updateMouseRotation(player, mousePos.x, mousePos.y, dim.x, dim.y);
        player->bulletTimer.update(deltaTime);  // Update timer with deltaTime
    }

    if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && !isPaused)
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

    if (!isPaused)
    {
        input->keyPressed(player, sounds, deltaTime);
        input->keyUp(player, sounds);
    }
}

void _scene::updateEnemySpawning()
{
    if (elapsedTime >= 1200.0f)  // Stop spawning after 20 minutes
        return;

    if (elapsedTime - lastSpawnTime >= spawnInterval)
    {
        if (maxEnemies < 300)  // Scale max enemies over time
            maxEnemies++;

        vec3 randPos;
        float angle = (rand() % 360) * (M_PI / 180.0f);
        float distance = 17.0f; // Spawn distance from player

        randPos.x = player->playerPosition.x + cos(angle) * distance;
        randPos.y = player->playerPosition.y + sin(angle) * distance;

        randPos.z = 48.0f;

        for (int i = 0; i < maxEnemies; i++)
        {
            if (!enemies[i].isAlive)  // Reuse dead enemies
            {
                enemies[i].placeEnemy(randPos);
                enemies[i].isAlive = true;
                enemies[i].initEnemy("images/swarmbot.png");
                enemies[i].setPlayerReference(player);
                lastSpawnTime = elapsedTime;
                break;
            }
        }

        // Adjust spawn rate dynamically
        spawnInterval = fmax(0.5f, 2.0f - elapsedTime / 600.0f);  // Faster spawns over time
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
