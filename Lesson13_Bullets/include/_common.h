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
using namespace std;

typedef struct
{
    float x,y,z;
}vec3;

typedef struct
{
    float x,y;
}vec2;

enum Action
{
    IDLE,
    RUN,
    ATTACK
};

struct AnimationData
{
    int framesX;          // Number of frames in the sprite sheet (1 row)
    int frameDuration;    // Time per frame in ms
    char* spriteSheet;    // Path to the sprite sheet file
};

#endif // _COMMON_H
