#include "_particles.h"

_particles::_particles()
{
    xMin = yMin = 0.0;
    xMax = yMax = 1.0;
}

_particles::~_particles()
{

}

void _particles::initParticles(vec3 pos, char* fileName)
{
    textureLoader->loadTexture(fileName);
    for(int i=0;i<MAX_PARTICLES;i++)
    {
        drops[i].position = pos;
    }
}

void _particles::buildQuad()
{
    glScalef(1.0,1.0,1.0);
    glBegin(GL_QUADS);
        glTexCoord2f(xMin,yMax);    glVertex3f(-1,-1,1);
        glTexCoord2f(xMax,yMax);    glVertex3f(1,-1,1);
        glTexCoord2f(xMax,yMin);    glVertex3f(1,1,1);
        glTexCoord2f(xMin,yMin);    glVertex3f(-1,1,1);
    glEnd();
}

void _particles::drawParticles()
{
    textureLoader->textureBinder();
    for(int i=0;i<totalDrops;i++)
    {
        if(drops[i].isAlive)
        {
            glPushMatrix();
                buildQuad();
            glPopMatrix();
        }
    }
}

void _particles::updateParticles()
{
    if(timer->getTicks()>50)
    {
        for(int i = 0; i <totalDrops; i++)
        {
            if(drops[i].isAlive)
            {
                drops[i].position.x += drops[i].velocity.x;
                drops[i].position.y += drops[i].velocity.y;

                drops[i].color.a -=1;
                drops[i].time += 0.00001;
            }

            if(drops[i].color.a<0)
            {
                drops[i].isAlive = false;
                drops[i].time = 0;
            }
        }

        timer->reset();
    }

}

void _particles::generateParticles()
{
    newDrops = rand()%+1;

    if(newDrops+totalDrops > MAX_PARTICLES)
    {
        newDrops = MAX_PARTICLES-totalDrops;
    }

    for (int i = totalDrops; i<newDrops+totalDrops; i++)
    {
        drops[i].isAlive = true;
        drops[i].radius = 10;
        drops[i].explosionRadius = 0.1;
        drops[i].angle = rand()%360;
        drops[i].color.a = rand()%50;
        drops[i].time = 0;
        drops[i].velocity.x = sin(drops[i].angle)*drops[i].radius;
        drops[i].velocity.y = cos(drops[i].angle)*drops[i].radius;
        //drops[i].velocity.z = cos(drops[i].angle)*drops[i].radius;
    }
    totalDrops +=newDrops;
    totalDrops >= MAX_PARTICLES? totalDrops = 0:NULL;
}
