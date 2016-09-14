#ifndef _SHAPES_H
#define _SHAPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

#define LEN 8192  //  Maximum length of text string

void cube(double x,double y,double z,double dx,double dy,double dz,double th);
void box(double x,double y,double z,double dx,double dy,double dz,double th);
void star(double x,double y,double z, double dx,double dy,double dz);

#endif

