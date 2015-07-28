/*
 * AP_Math.h
 *
 *  Created on: 27-jul.-2015
 *      Author: Arne
 */

#ifndef STUBS_AP_MATH_H_
#define STUBS_AP_MATH_H_

#include <math.h>

// square
float sq(float v);

// sqrt of sum of squares
float pythagorous2(float a, float b);
float pythagorous3(float a, float b, float c);

/* The following three functions used to be arduino core macros */
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))



#endif /* STUBS_AP_MATH_H_ */
