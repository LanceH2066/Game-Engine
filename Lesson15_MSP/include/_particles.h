#ifndef _PARTICLES_H
#define _PARTICLES_H

#include<_textureloader.h>
#include<_timer.h>
#include<_common.h>


class _particles
{
    public:
        _particles();
        virtual ~_particles();

        int totalNumDrops =0;
        int newDrops=0;
        _textureLoader *myTex = new _textureLoader();
        _timer *myTime = new _timer();

        typedef struct
        {
            bool live;
            vec3 pos;
            vec3 dir;
            col4 color;
            float t=0;

            float mass;
            vec3 velocity;
            float radius;
            float expRadius;
            float angle;
        }perticle;

        float xMax,yMax,xMin,yMin;

        void initParticles(vec3 ,char*); //position & filename
        void BuildQuad(vec3);

        void drawParticles();
        void updateParticles();
        void generateParticles(float, float);

        perticle drops[MAX_DRPOS];

    protected:

    private:
};

#endif // _PARTICLES_H
