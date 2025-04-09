#include "_enms.h"

_enms::_enms()
{
    //ctor
    pos.x =0.0; pos.y =-0.65; pos.z = -5;
    scale.x =0.25;
    scale.y =0.25;

    rot.x =rot.y=rot.z =0;
    actionTrigger =1;
    speed = 0.01;

    xMin= 0;
    yMin= 0;
    frames =7;  //depends on the file

    xMax = 1.0/frames;
    yMax = 1.0/2.0;
}

_enms::~_enms()
{
    //dtor
}
void _enms::initEnms(char* fileName)
{
    myTex->loadTexture(fileName);
              // load static variable
}


void _enms::drawEnms(GLuint tx)
{
   if(isEnmsLive)
   {
      glBindTexture(GL_TEXTURE_2D, tx);
      glColor3f(1.0,1.0,1.0);
      //myTex->textureBinder();
    //  glBindTexture(GL_TEXTURE_2D, myTx);

      glPushMatrix();
        glTranslatef(pos.x,pos.y,-2);
        glRotatef(rot.x,1,0,0);
        glRotatef(rot.y,0,1,0);
        glRotatef(rot.z,0,0,1);

        glScalef(scale.x,scale.y,1.0);

         glBegin(GL_POLYGON);

          glTexCoord2f(xMin,yMin);
          glVertex3f(1.0,1.0,0);

          glTexCoord2f(xMax,yMin);
          glVertex3f(-1.0,1.0,0);

          glTexCoord2f(xMax,yMax);
          glVertex3f(-1.0,-1.0,0);

          glTexCoord2f(xMin,yMax);
          glVertex3f(1.0,-1.0,0);

         glEnd();

      glPopMatrix();
   }
}

void _enms::placeEnms(vec3 p)
{
  pos.x = p.x;
  pos.y = p.y;
  pos.z = p.z;
}

void _enms::actions()
{
    switch(actionTrigger)
    {
    case RIGHTWALK:  // walk right

        if(myTimer->getTicks()>60)
        {
            xMin +=1.0/frames;
            xMax +=1.0/frames;
            yMin =0.5;
            yMax =1.0;

            pos.x<=1?pos.x += speed:actionTrigger=1;

            pos.y = -0.65;

            myTimer->reset();
        }

        break;

     case LEFTWALK: // walk Left

         if(myTimer->getTicks()>60)
        {
            xMin +=1.0/frames;
            xMax +=1.0/frames;
            yMin =0.0;
            yMax =0.5;

            pos.x -= speed;
            pos.x>=-1?pos.x -= speed:actionTrigger=2 ;

            myTimer->reset();
        }

        break;

        case STAND: // walk Left

         if(myTimer->getTicks()>60)
        {
            xMin = 0;
            xMax =1.0/frames;
            yMin =0.0;
            yMax =0.5;

            actionTrigger=0 ;

            myTimer->reset();
        }
        break;

        case ROTATELEFT:
        if(myTimer->getTicks()>60)
        {
            xMin +=1.0/frames;
            xMax +=1.0/frames;
            yMin =0.5;
            yMax =1.0;

            theta = 30*PI/180.0;
            pos.x -= vel*t*cos(theta)/1500;
            pos.y += (vel*t*sin(theta)-0.5*GRAVITY*t*t)/300;

            if(pos.y >-0.75) t+=0.3;
            else{t=0; pos.y = -0.65;}

            if(pos.x < -3.2) actionTrigger=RIGHTWALK;

            myTimer->reset();
        }
            break;
        case ROTATERIGHT:
          if(myTimer->getTicks()>60)
        {
            xMin +=1.0/frames;
            xMax +=1.0/frames;
            yMin =0.0;
            yMax =0.5;

            theta = 30*PI/180.0;
            pos.x += vel*t*cos(theta)/1500;
            pos.y += (vel*t*sin(theta)-0.5*GRAVITY*t*t)/300;

            if(pos.y >-0.75) t+=0.3;
            else{t=0; pos.y = -0.65;}

            if(pos.x >3.2){ actionTrigger=LEFTWALK; pos.y =-0.65;}

            myTimer->reset();
        }



            break;

    }

}
