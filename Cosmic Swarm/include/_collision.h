#ifndef _COLLISION_H
#define _COLLISION_H

#include<_common.h>

class _collision
{
    public:
        _collision();
        virtual ~_collision();

        bool isRadialCollision(vec3 pos1, vec3 pos2 , float r1, float r2, float threshold);  // Radial Collision Check

        bool isLinearCollision(vec2 pos1, vec2 pos2);                       // Linear Collision Check`
        bool isPlaneCollision(vec2 pos1, vec2 pos2);                        // Plane Collision Check
        bool isCubicCollision(vec3 pos1, vec3 pos2);                        // Cubic Collision Check
        bool isSphereCollision(vec3 pos1, vec3 pos2, float r1, float r2, float threshold);   // Spherical Collision Check
        bool isBoxCollision(vec3 pos, vec3 minBounds, vec3 maxBounds);

        bool isOBBCollision(vec3 min1, vec3 max1, vec3 min2, vec3 max2);  // 2D OBB check (ignores z for now)

    protected:

    private:
};

#endif // _COLLISION_H
