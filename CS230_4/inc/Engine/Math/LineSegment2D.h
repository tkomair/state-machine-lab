// ---------------------------------------------------------------------------
#ifndef LINE_SEGMENT_2D_H
#define LINE_SEGMENT_2D_H
// ---------------------------------------------------------------------------

#include "Vector2.h"

/*!
\struct LineSegment2D
\brief	Structure to represent a line segment.
*/
struct LineSegment2D
{
	Vector2 mP0;		// Point on the line
	Vector2 mP1;		// Point on the line
	Vector2 mN;			// Line's normal
	float mNdotP0;		// To avoid computing it every time it's needed

	// @TODO
	/*!
		\brief		Constructs a dummy line segment. 
		\details	Sets all values to zero
	*/
	LineSegment2D();

	// @TODO
	/*!
		\brief	Constructs a 2D line segment's data using 2 points

		\details
			- Edge is defined as E = P1-P0
			- mN is the outward normal and is defined as mN = -E.Perp().Normalize();
			- mNdotP0 = the dot product of the normal with p0.
	*/
	LineSegment2D(const Vector2 & p0, const Vector2 &p1);
};



// @TODO
/*!
\brief	This function determines the distance separating a point from a line

\return	The distance. Note that the returned value should be:
- Negative if the point is in the line's inside half plane
- Positive if the point is in the line's outside half plane
- Zero if the point is on the line
*/
float StaticPointToStaticLineSegment(Vector2 *P, LineSegment2D *LS);

// @TODO
/*!
\brief	Given a point P and an array of LineSegment2D, determines if a point
is contained by all line segments (ie. inside half plane of all segments).

\return	true if the point is inside all line segments, false otherwise.
*/
bool PointInLineSegments(Vector2 *P, LineSegment2D *LS, u32 count);

// ---------------------------------------------------------------------------
#endif