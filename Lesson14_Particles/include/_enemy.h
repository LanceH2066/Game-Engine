#ifndef _ENEMY_H
#define _ENEMY_H

#include<_common.h>
#include<_timer.h>
#include<_textureLoader.h>

class _enemy
{
    public:
        _enemy();
        virtual ~_enemy();

        // Timer / Texture Loader
        _timer * enemyTimer = new _timer();
        _textureLoader * enemyTextureLoader = new _textureLoader();

        // Position / Rotation / Scale
        vec3 position;
        vec3 rotation;
        vec2 scale;

        // Animation
        float xMax, xMin, yMax, yMin;
        float speed;
        int framesX;
        int framesY;
        int actionTrigger;

        bool isAlive;

        // Methods
        void initEnemy(char* fileName);
        void drawEnemy(GLuint);
        void placeEnemy(vec3);
        void enemyActions();

        enum {IDLE,LEFTWALK,RIGHTWALK,ROTATELEFT,ROTATERIGHT,ATTACK}; // Enemy Actions

        // Gravity
        float v = 35;        // velocity
        float theta = 0;    // theta angle of the projectile
        float t = 0;        // time step
        // y = vtsin(theta)-0.5g*t*t
        // x = vtcos(theta)
    protected:

    private:
};

#endif // _ENEMY_H
