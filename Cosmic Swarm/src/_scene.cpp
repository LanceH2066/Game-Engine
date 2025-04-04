#include "_scene.h"

_lightSetting *myLight = new _lightSetting();
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

}

GLint _scene::initGL()
{
    // GL SETTINGS
    glClearColor(1.0,1.0,1.0,1.0);
    glClearDepth(1.0);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    myLight->setLight(GL_LIGHT0);
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

    // Camera follows the player
    vec3 cameraPos = {player->playerPosition.x, player->playerPosition.y, player->playerPosition.z + 20.0f};
    vec3 lookAtPos = {player->playerPosition.x, player->playerPosition.y, player->playerPosition.z};

    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, // Eye position (camera)
              lookAtPos.x, lookAtPos.y, lookAtPos.z, // Center position (where the camera looks)
              0, 1, 0); // Up vector (keeps camera upright)

    glPushMatrix();
        glDisable(GL_LIGHTING);
        prlx1->drawBackground(dim.x, dim.y, player->playerPosition);
        glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
        glDisable(GL_LIGHTING);
        player->drawPlayer();
        glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
        glDisable(GL_LIGHTING);
            player->bullets.erase
            (
                remove_if(player->bullets.begin(), player->bullets.end(), [](const _Bullet& b) { return !b.isAlive; }),
                player->bullets.end()
            );
            for (auto& bullet : player->bullets)
            {
                if (bullet.isAlive)
                {
                    bullet.update(deltaTime);
                    bullet.drawBullet();

                    vec3 bulletMin = bullet.getCollisionBoxMin();
                    vec3 bulletMax = bullet.getCollisionBoxMax();

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
                    if (debugMode) {
                    glPushMatrix();
                        glTranslatef(bullet.position.x, bullet.position.y, bullet.position.z);
                        glColor3f(0.0f, 1.0f, 0.0f);  // Green for bullets
                        glBegin(GL_LINE_LOOP);
                            glVertex3f(bulletMin.x - bullet.position.x, bulletMin.y - bullet.position.y, 50.0);
                            glVertex3f(bulletMax.x - bullet.position.x, bulletMin.y - bullet.position.y, 50.0);
                            glVertex3f(bulletMax.x - bullet.position.x, bulletMax.y - bullet.position.y, 50.0);
                            glVertex3f(bulletMin.x - bullet.position.x, bulletMax.y - bullet.position.y, 50.0);
                        glEnd();
                    glPopMatrix();
                }
                }
            }
        glEnable(GL_LIGHTING);
    glPopMatrix();

    player->playerActions();

    updateEnemySpawning();

    // KEEP ENEMIES FROM STACKING, CHECK FOR PLAYER COLLISION, DRAW ENEMIES, RUN ACTIONS
    glPushMatrix();
        glDisable(GL_LIGHTING);
            for (int i = 0; i < 20; i++)
            {
                for (int j = i + 1; j < 20; j++)
                {
                    vec3 diff =
                    {
                        enemies[i].position.x - enemies[j].position.x,
                        enemies[i].position.y - enemies[j].position.y,
                        0.0f
                    };

                    float distSq = diff.x * diff.x + diff.y * diff.y;
                    float minDist = 0.5f; // Minimum distance to maintain

                    if (distSq < (minDist * minDist) && distSq > 0.0f)
                    {
                        float dist = sqrt(distSq);
                        float pushForce = (minDist - dist) * 0.05f; // Push force factor

                        // Normalize direction and apply push
                        diff.x /= dist;
                        diff.y /= dist;

                        enemies[i].position.x += diff.x * pushForce;
                        enemies[i].position.y += diff.y * pushForce;
                        enemies[j].position.x -= diff.x * pushForce;
                        enemies[j].position.y -= diff.y * pushForce;
                    }
                }

                enemies[i].drawEnemy(enemies[i].enemyTextureLoader->tex);
                enemies[i].enemyActions(deltaTime);

                if (debugMode && enemies[i].isAlive) {
                glPushMatrix();
                    glTranslatef(enemies[i].position.x, enemies[i].position.y, enemies[i].position.z);
                    glColor3f(1.0f, 0.0f, 0.0f);  // Red for enemies
                    glBegin(GL_LINE_LOOP);
                        glVertex3f(enemies[i].getCollisionBoxMin().x - enemies[i].position.x, enemies[i].getCollisionBoxMin().y - enemies[i].position.y, 0);
                        glVertex3f(enemies[i].getCollisionBoxMax().x - enemies[i].position.x, enemies[i].getCollisionBoxMin().y - enemies[i].position.y, 0);
                        glVertex3f(enemies[i].getCollisionBoxMax().x - enemies[i].position.x, enemies[i].getCollisionBoxMax().y - enemies[i].position.y, 0);
                        glVertex3f(enemies[i].getCollisionBoxMin().x - enemies[i].position.x, enemies[i].getCollisionBoxMax().y - enemies[i].position.y, 0);
                    glEnd();
                glPopMatrix();
            }
            }
        glEnable(GL_LIGHTING);
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

    input->updateMouseRotation(player, mousePos.x, mousePos.y, dim.x, dim.y);

    // Check if left mouse button is pressed
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
    {
        player->shoot(worldMousePos, sounds);
    }

    // Toggle debug mode with 'D' key
    if (GetAsyncKeyState('F') & 0x8000) {
        static bool lastState = false;  // Debounce to prevent rapid toggling
        bool currentState = true;
        if (!lastState && currentState) {
            debugMode = !debugMode;
            printf("Debug mode: %s\n", debugMode ? "ON" : "OFF");
        }
        lastState = currentState;
    } else {
        static bool lastState = false;
        lastState = false;
    }

    input->keyPressed(player, sounds,deltaTime);
    input->keyUp(player, sounds);
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
        //spawnInterval = fmax(0.5f, 2.0f - elapsedTime / 600.0f);  // Faster spawns over time
        spawnInterval =0.5f;
    }
}

void _scene::updateDeltaTime()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    deltaTime = (float)(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
    lastTime = currentTime;

    elapsedTime += deltaTime; // Track total run time
}
