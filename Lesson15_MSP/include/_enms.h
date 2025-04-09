#ifndef _ENMS_H
#define _ENMS_H

#include<_common.h>
#include<_textureloader.h>
#include<_timer.h>

class _enms
{
    public:
        _enms();
        virtual ~_enms();

        _timer *myTimer = new _timer();
        _textureLoader *myTex = new _textureLoader();
        float xMax, xMin, yMax, yMin;
        float speed;

        vec3 pos; //positions of enms
        vec2 scale;

        vec3 rot; // for rotations
        int frames;
        int actionTrigger;

        enum{STAND,LEFTWALK,RIGHTWALK,ROTATELEFT, ROTATERIGHT};

        void initEnms(char *);
        void drawEnms(GLuint);
        void placeEnms(vec3);
        void actions();
        bool isEnmsLive = true;

        float vel=35;   // for velocity
        float theta=0; // theta angle of the projectile
        float t= 0;     // time step

        //y = vtsin(theta)- 0.5g*t*t
        //x = vtcod(theta)
    protected:

    private:
};

#endif // _ENMS_H
