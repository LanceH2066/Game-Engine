#ifndef _PARTICLES_H
#define _PARTICLES_H

#include<_common.h>
#include<_textureLoader.h>
#include<_timer.h>

class _particles
{
    public:
        _particles();
        virtual ~_particles();

        _textureLoader *textureLoader = new _textureLoader();
        _timer *timer = new _timer;

        int totalDrops, newDrops;

        void initParticles(vec3 pos, char* fileName);
        void buildQuad();
        void drawParticles();
        void updateParticles();
        void generateParticles();

        float xMax,yMax,xMin,yMin;

        typedef struct
        {
            bool isAlive;
            vec3 position;
            vec3 direction;
            Color color;

            // Time / Mass / Velocity / Radius
            float time=0;
            float mass;
            vec3 velocity;
            float radius;

            float explosionRadius;
            float angle;
        }particle;

        particle drops[MAX_PARTICLES];

    protected:

    private:
};

#endif // _PARTICLES_H
