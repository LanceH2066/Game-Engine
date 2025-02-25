#include "_parallax.h"
_parallax::_parallax()
{
    xMax = 1.0;
    xMin = 0.0;
    yMax = 1.0;
    yMin = 0.0;
    speed = 0.005;
    upDown = leftRight = true;
}

_parallax::~_parallax()
{

}

void _parallax::drawBackground(float width, float height)
{
    background->textureBinder();

    glBegin(GL_POLYGON);
        glTexCoord2f(xMin,yMax);
        glVertex3f(-width/height, -1,-30);

        glTexCoord2f(xMax,yMax);
        glVertex3f(width/height, -1,-30);

        glTexCoord2f(xMax,yMin);
        glVertex3f(width/height, 1,-30);

        glTexCoord2f(xMin,yMin);
        glVertex3f(-width/height, 1,-30);
    glEnd();
}

void _parallax::initParallax(char* fileName, float speed, bool upDown, bool leftRight)
{
    this->speed = speed;
    this->upDown = upDown;
    this->leftRight = leftRight;
    background->loadTexture(fileName);
}

void _parallax::scroll(string direction)
{

        if(tmr->getTicks()>3)
        {
            if(direction=="up" && upDown==true)
            {
                yMin -= speed;
                yMax -= speed;
            }
            if(direction=="down" && upDown==true)
            {
                yMin += speed;
                yMax += speed;
            }
            if(direction=="left" && leftRight==true)
            {
                xMin -= speed;
                xMax -= speed;
            }
            if(direction=="right" && leftRight==true)
            {
                xMin += speed;
                xMax += speed;
            }
        }
        tmr->reset();

}

void _parallax::autoScroll()
{

        if(tmr->getTicks()>3)
        {
            if(upDown==true)
            {
                yMin += speed;
                yMax += speed;
            }
            if(leftRight==true)
            {
                xMin -= speed;
                xMax -= speed;
            }
        }
        tmr->reset();

}
