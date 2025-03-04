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
        void initEnemy(int xFrames, int yFrames, char* fileName);
        void drawEnemy();
        void placeEnemy();
        void enemyActions();


    protected:

    private:
};

#endif // _ENEMY_H
