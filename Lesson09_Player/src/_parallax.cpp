#include "_parallax.h"

_parallax::_parallax()
{
    xMax = 1.0;
    xMin = 0.0;
    yMax = 1.0;
    yMin = 0.0;
    speed = 0.005;
}

_parallax::~_parallax()
{

}

void _parallax::drawBackground(float width, float height)
{
    glColor3f(1.0,1.0,1.0);
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

void _parallax::initParallax(char* fileName)
{
    background->loadTexture(fileName);
}

void _parallax::scroll(string direction)
{

        if(tmr->getTicks()>3)
        {
            if(direction=="up")
            {
                yMin -= speed;
                yMax -= speed;
            }
            if(direction=="down")
            {
                yMin += speed;
                yMax += speed;
            }
            if(direction=="left")
            {
                xMin -= speed;
                xMax -= speed;
            }
            if(direction=="right")
            {
                xMin += speed;
                xMax += speed;
            }
        }
        tmr->reset();

}
