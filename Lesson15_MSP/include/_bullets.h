#ifndef _BULLETS_H
#define _BULLETS_H

#include<_bullets.h>
#include<_timer.h>
#include<_textureloader.h>

class _bullets
{
    public:
        _bullets();
        virtual ~_bullets();

        _textureLoader *myTex = new _textureLoader();
        _timer *myTime = new _timer();

        enum{IDLE,SHOOT,HIT};
        vec3 bPos;
        vec3 bDes;
        vec3 bRot;
        vec3 bScale;

        bool bLive;
        int actionTrigger;
        int t=0; //
        float xMin,xMax,yMin,yMax;

        void bInit(vec3);
        void bReset(vec3);
        void bUpdate(vec3,vec3);
        void bActions();
        void drawBullet(GLuint);


    protected:

    private:
};

#endif // _BULLETS_H
