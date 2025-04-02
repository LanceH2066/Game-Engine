#ifndef _BULLETS_H
#define _BULLETS_H

#include<_bullets.h>
#include<_timer.h>
#include<_textureloader.h>
#include<_player.h>
#include<_sounds.h>
class _bullets
{
    public:
        _bullets();
        virtual ~_bullets();

        _textureLoader *myTex = new _textureLoader();
        _timer *myTime = new _timer();

        enum{IDLE,SHOOT,HIT};
        vec3 bPos;
        vec3 initialPos;
        vec3 bDes;
        vec3 bRot;
        vec3 bScale;
        vec3 velocity;

        bool bLive;
        int actionTrigger;
        float t = 0.0f;
        int bounceCount = 0;
        float xMin, xMax, yMin, yMax;
        float animTime = 0.0f;

        void bInit(_player *player);
        void bReset(vec3);
        void bUpdate(vec3, vec3,_sounds*);
        void bActions();
        void drawBullet(GLuint);
        _player* playerRef;
        void fireBullet();

    protected:
    private:
};

#endif // _BULLETS_H
