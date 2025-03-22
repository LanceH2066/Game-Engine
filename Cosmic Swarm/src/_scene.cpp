#include "_scene.h"

_lightSetting *myLight = new _lightSetting();
_inputs *input = new _inputs();
_parallax *prlx1 = new _parallax();
_player *player = new _player();
_enemy enemies[20];
_collision *collision = new _collision();
_sounds *sounds = new _sounds();
_Bullet bullets[20];

_scene::_scene()
{

}

_scene::~_scene()
{

}

GLint _scene::initGL()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    myLight->setLight(GL_LIGHT0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

    prlx1->initParallax("images/background.png", 0.005, false, false);

    player->initPlayer(1,1,"images/player.png");
    /*
    enemies[0].initEnemy("images/Sprites/mon.png");
    bullets[0].textureLoader->loadTexture("images/bullet.png");

    for(int i =0; i < 20; i++)
    {
        vec3 randPos = {(float)rand()/(float)(RAND_MAX)*5-2.5,-0.8,-2};
        enemies[i].position = randPos;
        enemies[i].speed = (float)((rand()%8)+1.0)/100.0;

        bullets[i].init(player->playerPosition);
    }
    */

    sounds->initSounds();
    sounds->playMusic("sounds/music.mp3");

    return true;
}

void _scene::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clear Bits
    glLoadIdentity();                                 // Identity Matrix

    // Camera follows the player
    vec3 cameraPos = {player->playerPosition.x, player->playerPosition.y, player->playerPosition.z + 20.0f};
    vec3 lookAtPos = {player->playerPosition.x, player->playerPosition.y, player->playerPosition.z};

    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, // Eye position (camera)
              lookAtPos.x, lookAtPos.y, lookAtPos.z, // Center position (where the camera looks)
              0, 1, 0); // Up vector (keeps camera upright)

    glPushMatrix();
        glDisable(GL_LIGHTING);
        prlx1->drawBackground(dim.x, dim.y, player->playerPosition); // Move background with player
        //prlx2->drawBackground(dim.x,dim.y);
        glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
        glDisable(GL_LIGHTING);
        player->drawPlayer();
        glEnable(GL_LIGHTING);
    glPopMatrix();

    /*
    glPushMatrix();
        glDisable(GL_LIGHTING);
        for(int i =0; i < 20; i++)
        {
            if(collision->isRadialCollision(enemies[i].position,player->playerPosition,0.1,0.2,0.02))
            {
                if(enemies[i].actionTrigger==enemies[i].RIGHTWALK)
                {
                    enemies[i].actionTrigger = enemies[i].ROTATELEFT;
                }
                else if(enemies[i].actionTrigger==enemies[i].LEFTWALK)
                {
                    enemies[i].actionTrigger = enemies[i].ROTATERIGHT;
                }
            }

            enemies[i].drawEnemy(enemies[0].enemyTextureLoader->tex);
            enemies[i].enemyActions();

            bullets[i].drawBullet(bullets[0].textureLoader->tex);
        }
        glEnable(GL_LIGHTING);

    glPopMatrix();
    */
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
    input->keyPressedPlayer(player);
    input->keyUpPlayer(player);
}
