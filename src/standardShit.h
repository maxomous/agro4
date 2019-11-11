//standardShit.h

#ifndef 	__STANDARDSHIT_H__
#define 	__STANDARDSHIT_H__

#include	<stdio.h>
//~ #include 	<stdlib.h>
//~ #include 	<stdarg.h>				// allows variable number arguments
#include 	<string.h>				// string functions
//~ #include 	<assert.h>				// memory allocation functions
//~ #include 	<ctype.h>				// strings functions

//~ #include 	<float.h>
#include 	<math.h>				// math functions
#include 	<time.h>				// time functions

#include 	<GL/glew.h>				// Used for vbos
#include 	<GL/freeglut.h>			// OpenGL GUI
#define		GL_GLEXT_PROTOTYPES
#define 	TRUE 1
#define 	FALSE 0
#define 	PI 		3.141592654
#define 	Deg2Rad(degs)      (((degs) * PI) / 180.0)
#define		Rad2Deg(radians)    ((radians) * (180.0 / PI))
#define 	MILLION 			1E6
#define 	BILLION 			1E9
#define		MAX_STRING 32
typedef struct{
	
	float x;
	float y;
	
}point2D; 

float currentTime;

extern double AngleFrom2Points(point2D centre, point2D end);
extern point2D PointFromPolar(point2D centre, double r, double angle);
extern double GetTimeMono();
extern double CleanUpAngle(double Angle);
extern double Distance(point2D p0, point2D p1);
extern void CleanUpAngles(double *StartAngle, double *EndAngle, int Direction);
extern point2D add2D(point2D p1, point2D p2);

#endif
