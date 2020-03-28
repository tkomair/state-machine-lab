#ifndef CS230_PATH2D_H_
#define CS230_PATH2D_H_
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//!	\class	PathPoint2D
//!	\brief	Describes a point for a path, associated to a time mTime.
struct PathPoint2D
{
	// ------------------------------------------------------------------------
	// DATA

	//! Time associated to the point on the path.
	f32		mTime;

	//! 2D Point on the path (in world space). 
	Vector2 mPoint;

	// ------------------------------------------------------------------------
	// METHODS

	// Constructors

	// @TODO
	//! Sets time and point to zero. 
	PathPoint2D();

	// @TODO
	//! Sets time to t and point to zero. 
	PathPoint2D(f32 t);

	// @TODO
	//! Sets time to t and point to pt. 
	PathPoint2D(f32 t, const Vector2 & pt);

	// @PROVIDED
	//! Debug prints to std::cout
	void Print() const;

	// @PROVIDED
	//! Writes the point to stream
	void ToStream(std::ostream & str) const;

	// @PROVIDED
	//! Reads the point from stream
	void FromStream(std::istream &str);
};

// ----------------------------------------------------------------------------
/*!
	\class	Path2D
	\brief	Class that holds several path points and provides methods for
			moving along a path. 
*/
struct Path2D
{
	// ------------------------------------------------------------------------
	// DATA
	
	//!	Points on the path associated with a time
	std::vector<PathPoint2D> mPoints;	

	//! Length of the entire path at each segment. Note that this is the 
	// CUMMULATED length, meaning that each item i in this vector is equal to 
	// the distance between mPoints[i] and mPoints[i+1] + the length of the 
	// previous segment. Thus, mCummulatedLengths[i] = mCummulatedLengts[i-1] + Distance(mPoints[i], mPoints[i+1]);
	// Note that this means that the total length of the path corresponds to the last
	// item in that vector. 
	std::vector<f32> mCummulatedLengths;	

	// ------------------------------------------------------------------------
	// METHODS
	
	// @TODO
	//! Gets point on the path from a given time t.
	Vector2 SampleTime(f32 t) const;				

	// @TODO
	//! Gets a point on the path given the distance from the start of the path. 
	Vector2 SampleDistance(f32 dist) const;

	// @TODO
	//! Gets a point on the path from a normalized distance parameter
	Vector2 SampleParameter(f32 u)const;			

	// @TODO
	//! Adds a new point on the path.
	void Push(f32 t, const Vector2 &val);

	// @TODO
	//! Removes all points and segments.
	void Clear();

	// @TODO
	//! Returns the number of points in the path.
	u32 Size() const;

	// @TODO
	//! Returns the duration of the whole path.
	f32 Duration() const;

	// @TODO
	//! Returns the total length of the path
	f32 GetTotalLength() const;

	// @TODO
	//! Returns the length from the start to the point represented
	//	by the normalized parameter u. (Hint: Use this function in 
	//	SampleParameter() function above).
	f32 GetLengthFromParam(f32 u)const;

	// @PROVIDED
	//!	Writes the path to a stream (file, string,etc...).
	void ToStream(std::ostream & str) const;

	// @PROVIDED
	//!	Reads the path from a stream.
	void FromStream(std::istream &str);
};

// ----------------------------------------------------------------------------
#endif