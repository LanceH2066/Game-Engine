#include "_particles.h"

_particles::_particles()
{
    //ctor
    xMax = yMax =1.0;
    xMin = yMin =0.0;
}

_particles::~_particles()
{
    //dtor
}
void _particles::initParticles(vec3 pos, char* fileName)
{
    myTex->loadTexture(fileName);
    for(int i=0;i<MAX_DRPOS; i++)
    {
        drops[i].pos = pos;
    }
}

void _particles::BuildQuad(vec3 pos)
{
   glTranslatef(pos.x,pos.y,pos.z);
   glScalef(0.01,0.01,1.0); // size it depends on your scene


    glBegin(GL_QUADS);
     glTexCoord2f(xMin,yMax);glVertex3f(-1,-1,1);
     glTexCoord2f(xMax,yMax);glVertex3f( 1,-1,1);
     glTexCoord2f(xMax,yMin);glVertex3f( 1, 1,1);
     glTexCoord2f(xMin,yMin);glVertex3f(-1, 1,1);
    glEnd();
}

void _particles::drawParticles()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    myTex->textureBinder();
    for(int i =0; i<totalNumDrops;i++)
    {
        if(drops[i].live)
        {
            glPushMatrix();
            BuildQuad(drops[i].pos);
            glPopMatrix();
        }
    }
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void _particles::updateParticles()
{
    if(myTime->getTicks()>50)
    {
        for(int i =0; i<totalNumDrops;i++)
        {
           if(drops[i].live)
           {
               drops[i].pos.x += drops[i].velocity.x;
               drops[i].pos.y += drops[i].velocity.y;

               drops[i].color.alpha -=1;
               drops[i].t +=0.0001;

               if(drops[i].color.alpha <0)
               {
                   drops[i].live = false;
                   drops[i].t =0;
               }
           }
       }
       myTime->reset();
    }
}

void _particles::generateParticles(float mx, float my)
{
    newDrops = rand()%30+1;

    if(newDrops+totalNumDrops >MAX_DRPOS)
        newDrops= MAX_DRPOS - totalNumDrops;

    for(int i= totalNumDrops; i<newDrops+totalNumDrops;i++)
    {
        drops[i].live = true;
        drops[i].radius =1;
        drops[i].pos.x = mx;
        drops[i].pos.y = my;
        drops[i].pos.z = -1.8;//(rand()%50/100.0)-5.0;

        drops[i].expRadius = 0.05;
        drops[i].angle = rand()%360;

        drops[i].velocity.x = sin(drops[i].angle)*drops[i].expRadius;
        drops[i].velocity.y = cos(drops[i].angle)*drops[i].expRadius;
        drops[i].velocity.z = cos(drops[i].angle)*drops[i].expRadius;

        drops[i].color.alpha = rand()%50;
        drops[i].t =0;
    }
    totalNumDrops +=newDrops;
    totalNumDrops>=MAX_DRPOS? totalNumDrops=0:NULL;
}
