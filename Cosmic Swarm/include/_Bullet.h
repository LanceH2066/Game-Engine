#ifndef _BULLET_H
#define _BULLET_H

#include<_common.h>
#include<_textureLoader.h>
#include<_timer.h>

class _Bullet
{
    public:
        _Bullet();
        virtual ~_Bullet();
        // TEXTURES
        _textureLoader *textureLoader = new _textureLoader();
        float xMin,xMax,yMin,yMax;
        // TIME
        _timer *timer = new _timer();
        int t=0;
        // ACTIONS
        enum{IDLE,SHOOT,HIT};

        int actionTrigger;
        bool isAlive = false;
        // TRANSFORM
        vec3 initialPosition, position, direction, rotation, scale;
        vec3 offset;  // Offset from center for collision purposes
        bool isLeftBullet;  // To identify left vs right bullet

        // METHODS
        void init(vec3 playerPos, vec3 playerRotation, vec3 targetPos, char * fileName, bool isLeft = false);
        void reset(vec3);                           // Reset on hit
        void update(float deltaTime);               // Update bullet
        void actions();                             // Bullet actions
        void drawBullet();                          // Draw Bullet

        float damage = 5.0f;

    protected:

    private:
};

#endif // _BULLET_H
