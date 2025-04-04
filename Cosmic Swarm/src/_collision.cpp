#include "_collision.h"

_collision::_collision()
{

}

_collision::~_collision()
{

}

bool _collision::isOBBCollision(vec3 min1, vec3 max1, vec3 min2, vec3 max2)
{
    // Simple AABB check first for early rejection (assuming z is ignored for 2D gameplay)
    if (max1.x < min2.x || min1.x > max2.x || max1.y < min2.y || min1.y > max2.y) {
        return false;
    }
    return true;  // For now, assume overlap if AABB passes; refine with full OBB later if needed
}

bool _collision::isRadialCollision(vec3 pos1, vec3 pos2, float r1, float r2, float threshold)
{
    return ((sqrt(pow((pos1.x-pos2.x),2)+pow((pos1.y-pos2.y),2))-(r1+r2))<threshold);
}


bool _collision::isLinearCollision(vec2 pos1, vec2 pos2)
{
    return false;
}

bool _collision::isPlaneCollision(vec2 pos1, vec2 pos2)
{
    return false;
}

bool _collision::isCubicCollision(vec3 pos1, vec3 pos2)
{
    return false;
}

bool _collision::isSphereCollision(vec3 pos1, vec3 pos2, float r1, float r2, float threshold)
{
    return (sqrt(pow((pos1.x-pos2.x),2)+pow((pos1.y-pos2.y),2)+pow((pos1.z-pos2.z),2))-(r1+r2))<threshold;
}

bool _collision::isBoxCollision(vec3 pos, vec3 minBounds, vec3 maxBounds)
{
    return (pos.x >= minBounds.x && pos.x <= maxBounds.x &&
            pos.y >= minBounds.y && pos.y <= maxBounds.y);
}
