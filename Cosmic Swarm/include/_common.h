#ifndef _COMMON_H
#define _COMMON_H

#include <windows.h>
#include <Winuser.h>
#include<iostream>
#include <algorithm>
#include<stdlib.h>
#include<vector>
#include<gl/gl.h>
#include<GL/glut.h>
#include<time.h>
#include<math.h>
#include<string>
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

#endif // _COMMON_H
