#include <AEEngine.h>		// Alpha Engine
#include <GAM150.h>			// GAM 150 Labs
#include <Engine/Engine.h>		// CS 230 HW 4
#include <Demos/Common.h>		// CS 230 HW 4
#include "DemoActor.h"

void DemoCamera::Update()
{
	if (mTarget && mCamera) {
		// go from previous to current with drag
		if (matchPos) mCamera->mTransform.mPosition += mTarget->mPosition - mCamera->mTransform.mPosition * mTension;
		if (matchRot) mCamera->mTransform.mOrientation += mTarget->mRotation - mCamera->mTransform.mOrientation * mTension;
		if (matchScale) mCamera->mTransform.mScale += mTarget->mScale - mCamera->mTransform.mScale * mTension;
	}
}
