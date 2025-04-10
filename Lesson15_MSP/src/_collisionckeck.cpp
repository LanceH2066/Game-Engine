#include "_collisionckeck.h"

_collisionCkeck::_collisionCkeck()
{
    //ctor
}

_collisionCkeck::~_collisionCkeck()
{
    //dtor
}
bool _collisionCkeck::isLinearCol(vec2, vec2)
{

}

bool _collisionCkeck::isRadialCol(vec3 p1, vec3 p2, float r1, float r2, float thrhld)
{
   return (sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2))-(r1+r2)<thrhld);
}

bool _collisionCkeck::isPlaneCol(vec2, vec2)
{

}

bool _collisionCkeck::isCubicCol(vec3, vec3)
{

}

bool _collisionCkeck::isSphereCol(vec3 p1, vec3 p2, float r1, float r2, float thrhld)
{
   return (sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2)+pow((p1.z-p2.z),2))-(r1+r2)> thrhld);

}
