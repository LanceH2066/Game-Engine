#ifndef _SCENE_H
#define _SCENE_H

#include<_lightSetting.h>
#include<_model.h>
#include<_inputs.h>
#include<_parallax.h>
#include<_player.h>
#include<_enemy.h>
#include<_collision.h>
#include<_sounds.h>
#include<_Bullet.h>
#include<_xporb.h>

class _scene
{
    public:

        _scene();                               // Constructor
        virtual ~_scene();                      // Destructor

        GLint initGL();                         // Initialize Game Objects
        void drawScene();                       // Render The Final Scene
        void reSize(GLint width, GLint height); // Resize Window

        void updateDeltaTime(); // New function to update delta time
        void processKeyboardInput();
        void updateEnemySpawning();
        vec2 dim; // for screen width/height
        vector<_enemy> enemies;
        //xpOrbs
        vector<_xpOrb> xpOrbs;
    protected:

    private:

    float elapsedTime = 0.0f;  // Tracks total run time
    float spawnInterval = 0.05f; // Initial time between spawns
    float lastSpawnTime = 0.0f; // Time since last spawn
    int maxEnemies = 100;  // Start with a limit

    LARGE_INTEGER frequency;
    LARGE_INTEGER lastTime;
    float deltaTime;

    bool debugMode = false;  // Add this for toggling hitbox drawing
    bool isPaused = false;  // Add this for pausing
    bool gameOver = false;  // Add this for game-over state
    float damageCooldown = 0.0f;  // Cooldown timer to prevent rapid damage
};

#endif // _SCENE_H
