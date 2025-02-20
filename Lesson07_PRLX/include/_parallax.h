#ifndef _PARALLAX_H
#define _PARALLAX_H

#include<_common.h>
#include<_textureLoader.h>

class _parallax
{
    public:
        _parallax();
        virtual ~_parallax();

        _textureLoader *background = new _textureLoader();

        void drawBackground(float,float);   // pass width and height
        void initParallax(char*);           // File Name
        void scroll(bool,string,float);     // Auto move, direction, speed

        float xMax, xMin, yMax, yMin;       // Texture coordinates

    protected:

    private:
};

#endif // _PARALLAX_H
