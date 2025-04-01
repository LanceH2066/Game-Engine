#ifndef _COMMON_H
#define _COMMON_H

#include<windows.h>
#include<iostream>
#include<stdlib.h>
#include<gl/gl.h>
#include<GL/glut.h>
#include<time.h>
#include<math.h>
#include<time.h>
#include<map>
#define PI 3.14159
#define GRAVITY 9.81
#define MAX_PARTICLES 5000

using namespace std;

typedef struct
{
    float x,y,z;
}vec3;

typedef struct
{
    float x,y;
}vec2;

typedef struct
{
    float r;
    float g;
    float b;
    float a;
}Color;

#endif // _COMMON_H
