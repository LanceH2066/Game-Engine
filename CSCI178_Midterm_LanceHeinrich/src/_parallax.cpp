#include "_parallax.h"

_parallax::_parallax()
{
    //ctor

    xMax = 1.0;
    xMin = 0.0;
    yMax = 1.0;
    yMin = 0.0;
    speed = 0.005;
}

_parallax::~_parallax()
{
    //dtor
}
void _parallax::drawBackground(float width, float height)
{
    glColor3f(1.0,1.0,1.0);
    background->textureBinder();

    glBegin(GL_POLYGON);
      glTexCoord2f(xMin,yMax);
      glVertex3f(-width/height,-1,-30);

      glTexCoord2f(xMax,yMax);
      glVertex3f(width/height,-1,-30);

      glTexCoord2f(xMax,yMin);
      glVertex3f(width/height,1,-30);

      glTexCoord2f(xMin,yMin);
      glVertex3f(-width/height,1,-30);
    glEnd();
}

void _parallax::initPrlx(char* fileName)
{
    background->loadTexture(fileName);
}

void _parallax::scroll(bool Auto, string dir, float speed)
{
 //  if(Auto)
   {
       if(tmr->getTicks()>50)
       {

       if(dir=="up")
       {
           yMin +=speed;
           yMax +=speed;
       }

       if(dir=="down")
       {
           yMin -=speed;
           yMax -=speed;
       }
       if(dir=="right")
       {
           xMin +=speed;
           xMax +=speed;
       }
       if(dir=="left")
       {
           xMin -=speed;
           xMax -=speed;
       }
       tmr->reset();
       }// end timer check
   }
}
