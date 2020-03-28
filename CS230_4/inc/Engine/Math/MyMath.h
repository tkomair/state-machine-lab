#ifndef CS230_MATH_H_
#define CS230_MATH_H_

#include <math.h>	/*sin and cos*/

#ifndef EPSILON
#define	EPSILON	0.00001f
#endif

#ifndef FLOAT_ZERO
#define FLOAT_ZERO(x) (x >-EPSILON && x < EPSILON)
#endif

#ifndef PI
#define	PI		3.1415926535897932384626433832795f
#endif

#ifndef HALF_PI
#define	HALF_PI	(PI * 0.5f)
#endif

#ifndef TWO_PI
#define	TWO_PI	(PI * 2.0f)
#endif


// Assignment 2
#include "Vector2.h"
#include "Matrix33.h"
#include "Transform2D.h"
#include "Collisions.h"

// Assignment 3
#include "Polygon2D.h" // extra credit
#include "ContactCollisions.h"

// Assignment 4
#include "LineSegment2D.h"
#include "Raycast.h"

#endif