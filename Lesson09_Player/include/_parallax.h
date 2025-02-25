#ifndef _PARALLAX_H
#define _PARALLAX_H

#include<_common.h>
#include<_textureLoader.h>
#include<_timer.h>

class _parallax
{
    public:
        _parallax();
        virtual ~_parallax();

        _textureLoader *background = new _textureLoader();
        _timer *tmr = new _timer();

        void drawBackground(float,float);   // pass width and height
        void initParallax(char*);           // File Name
        void scroll(string);     // Auto move, direction, speed

        float xMax, xMin, yMax, yMin;       // Texture coordinates
        float speed;

    protected:

    private:
};

#endif // _PARALLAX_H
