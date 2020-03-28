#ifndef PHYSICS_OBJ_H_
#define PHYSICS_OBJ_H_
#include <GameObject.h>
#include "..\Math\MyMath.h"
enum ECollisionShape
{
	CSHAPE_AABB		= 1,	// 001
	CSHAPE_CIRCLE	= 2,	// 010
	CSHAPE_OBB		= 4,	// 100
	CSHAPE_INDEX_MAX = (CSHAPE_OBB | CSHAPE_CIRCLE) + 1
};
struct RigidBody : public GameObject
{
	// physics data
	Vector2	mPrevPosition;
	Vector2	mVelocity;
	Vector2	mAcceleration;
	Vector2	mGravity;
	f32		mInvMass;
	f32		mDrag;

	ECollisionShape mCollisionShape = CSHAPE_AABB;
	
	// NEW for ASSIGNMENT 4 demo!

	// true if has collided this frame
	bool mbHasCollided;

	// if collided this frame, contains rigidbodies it collided with
	std::vector<RigidBody*> mCollidedWith;	

	//  ------------------------

	// constructor
	RigidBody();

	// integrate
	void Integrate(float timeStep);

	// adding force
	void AddForce(Vector2 force);

	// compute AABB (center + size)
	void ComputeAABB(Vector2 * outPos, Vector2 * outSize);

	// update
	virtual void Update();

	// render (Provided)
	virtual void Render(AEMtx33 * pCamMtx = NULL);
};



#endif