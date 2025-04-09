#ifndef _COMMON_H
#define _COMMON_H

#include<windows.h>
#include<iostream>
#include<gl/gl.h>
#include<GL/glut.h>
#include<time.h>
#include<math.h>
#include<time.h>

#define PI 3.14159
#define GRAVITY 9.81
#define MAX_DRPOS 3000

using namespace std;

typedef struct{

 float x;
 float y;
 float z;
}vec3;

typedef struct{

 float x;
 float y;
}vec2;

typedef struct{
  float r;
  float g;
  float b;
  float alpha;
} col4;

#endif // _COMMON_H
