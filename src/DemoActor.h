#pragma once
#include <Engine/AI/StateMachine.h>
#include <Engine/Animation/Path2D.h>
struct DemoActor : public Actor
{
	Path2D mPath;
	Vector2 mPathPosition = { 100, 100 };
	float mMoveSpeed = 170.0f;
	void Initialize();
	void Update();
};

struct GameObject;
struct DemoCamera : public Actor
{
	DemoCamera(Camera* cam, GameObject* tgt)
		: Actor(), mCamera(cam), mTarget(tgt) {}

	GameObject*		mTarget= nullptr;
	Camera*			mCamera = nullptr;
	f32				mTension = 1.0f;
	bool			matchPos = true;
	bool			matchRot = false;
	bool			matchScale = false;
	void			Update();
};