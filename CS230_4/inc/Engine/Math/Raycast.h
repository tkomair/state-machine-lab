#ifndef RAYCAST_H_
#define RAYCAST_H_

/*!
\struct Ray
\brief	 Represents a 2-dimensional ray with an origina and direction.
\remark Direction is not assumed to be normalized.
*/
struct Ray
{
	Vector2 mOrigin;
	Vector2 mDirection;
};

// @TODO
//! Performs a raycast test against a line.
f32 RayCastLine(const Vector2 & origin, const Vector2 & dir, const LineSegment2D & line, Vector2 * outPi);
// @TODO
//! Performs a raycast test against an obb.
f32 RayCastRect(const Vector2 & origin, const Vector2 & dir, const Transform2D & rect, Vector2 * outPi);
// @TODO
//! Performs a raycast test against a circle.
f32 RayCastCircle(const Vector2 & origin, const Vector2 & dir, const Vector2 & circle, f32 radius, Vector2 * outPi);

// @PROVIDED
//! Performs a raycast test against a line.
f32 RayCastLine(const Ray & ray, const LineSegment2D & line, Vector2 * outPi);
// @PROVIDED
//! Performs a raycast test against an obb.
f32 RayCastRect(const Ray & ray, const Transform2D &rect, Vector2 * outPi);
// @PROVIDED
//! Performs a raycast test against a circle.
f32 RayCastCircle(const Ray & ray, const Vector2 & circle, f32 radius, Vector2 * outPi);

#endif