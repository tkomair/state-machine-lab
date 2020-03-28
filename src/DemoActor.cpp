#include <AEEngine.h>		// Alpha Engine
#include <GAM150.h>			// GAM 150 Labs
#include <Engine/Engine.h>		// CS 230 HW 4
#include <Demos/Common.h>		// CS 230 HW 4
#include <Demos/CurveEditor.h>  // CS 230 HW 4
#include "DemoActor.h"
#include <fstream>
struct SM_Wait;
struct SM_PickTarget;
struct SM_GoToTarget;

struct SM_Wait : public State
{
	SM_Wait() : State("Wait") {}
	f32 waitDuration = 1.0f; // seconds

	void LogicEnter() {
		mActor->mColor = AE_COLORS_BLUE;
	}
	void LogicUpdate() {
		if (mTimeInState >= waitDuration)
			mOwnerStateMachine->ChangeState("Pick Target");
	}
};
struct SM_GoToTarget : public State
{
	State* returnState = NULL;
	Vector2 mTarget;
	SM_GoToTarget() : State("Go To Target") {}
	void LogicEnter() {
		mActor->mColor = AE_COLORS_YELLOW;
	}
	void LogicExit() {
		returnState = NULL;
	}
	void LogicUpdate(){
		Vector2 distance = (mTarget - mActor->mPosition);
		mActor->mPosition +=  distance * 0.05f;

		// debug draw the target and the distance left. 
		AEGfxSetTransform(&gCamera.GetWorldToCam());
		AEGfxCircle(mTarget.x, mTarget.y, 15.0f, AE_COLORS_BLACK);
		AEGfxLine(mActor->mPosition.x, mActor->mPosition.y, 0, AE_COLORS_BABY_BLUE,
			mTarget.x, mTarget.y, 0, AE_COLORS_BABY_BLUE);
		
		// close enough to Target
		if (distance.Length() < 5.0f) {
			if (returnState) 
				mOwnerStateMachine->ChangeState(returnState);
			else
				mOwnerStateMachine->ChangeState("Wait");
		}
	}
};
struct SM_PickTarget : public State
{
	SM_PickTarget() : State("Pick Target") {}

	f32 waitDuration = 1.0f;
	void LogicEnter() {
		mActor->mColor = AE_COLORS_RED;
	}
	void LogicUpdate() {

		mActor->mRotation += AEDegToRad(50.0f) * (f32)gAEFrameTime;

		if (mTimeInState >= waitDuration) {


			SM_GoToTarget* state = dynamic_cast<SM_GoToTarget*>(mOwnerStateMachine->GetState("Go To Target"));
			if (state) {
				// generate the new path at a distance from the actor
				AEVec2 dist; dist.FromAngle(AERandFloat(0, TWO_PI)); dist *= AERandFloat(350, 1050);
				state->mTarget = mActor->mPosition + dist;
				mOwnerStateMachine->ChangeState("Go To Target");

				// 5/6 chance to go to path after reaching the target
				if (rand() % 6)
				{
					DemoActor* actor = (DemoActor*)mActor;

					// modify the target to the start of the path
					actor->mPathPosition = state->mTarget;		// set the path position to the new target value
					state->mTarget = actor->mPath.mPoints[0].mPoint + actor->mPathPosition; // update target to go to the first point
					state->returnState = mOwnerStateMachine->GetState("Follow Path");
				}
			}
		}
	}
};

struct SM_FollowPath : public SuperState
{
	SM_FollowPath() :SuperState("Follow Path", NULL) {}
	Vector2 pathStart;
	void LogicEnter() {
		// not used anymore, here in case we need it later. 
	}
};

struct SM_OnPath : public State
{
	SM_OnPath() :State("On Path") {}
	Path2D* mPathPtr = NULL;
	float mCurrentDistance = 0;
	float mWaitDuration = 1.0f;
	void LogicEnter() {
		// get access to the path from the actor
		DemoActor* actor = (DemoActor*)mActor;
		mPathPtr = &actor->mPath;
		mCurrentDistance = 0.0f;
	}

	void LogicUpdate() {
		DemoActor* actor = (DemoActor*)mActor;
		mActor->mPosition = mPathPtr->SampleDistance(mCurrentDistance) + actor->mPathPosition;
		mCurrentDistance += actor->mMoveSpeed * (f32)gAEFrameTime;

		// loop over the whole path
		if (mCurrentDistance > mPathPtr->GetTotalLength())
			mCurrentDistance -= mPathPtr->GetTotalLength();

		// change state after some time
		if (mTimeInState > mWaitDuration)
			mActor->mBrain[0].ChangeState("Wait");
	}
};

struct SM_Navigate : public SuperState {
	SM_Navigate(StateMachine* wanderSM) 
		:SuperState("Navigate", NULL), mWanderSM(wanderSM){}

	StateMachine* mWanderSM = nullptr;
	void LogicUpdate()
	{
		if (!mWanderSM)return;

		// if we're about to change to the Go to Target, if so go to active
		if (mWanderSM->mCurrentState && mWanderSM->mNextState->mName == "Go To Target")
			mInternalStateMachine.ChangeState("Active");// this is ok, because if we're already in that state nothing will change

		// otherwise 
		else 
			mInternalStateMachine.ChangeState("Inactive");

	}
};


struct SM_Nav_Active : public State {
	SM_Nav_Active(StateMachine* wanderSM) 
		:State("Active"), mWanderSM(wanderSM) {}

	StateMachine* mWanderSM = nullptr;
	void LogicUpdate() {
		SM_GoToTarget* gotoState = (SM_GoToTarget*)mWanderSM->GetState("Go To Target");
		gotoState->mTarget = gCamera.WindowPointToWorld(gAEMousePosition);
	}
};

void DemoActor::Initialize()
{
	// initiailize Colliders
	InitializeColliders();

	// initialize as an Actor
	Actor::Initialize(); 

	// IMPORTANT: The state machine API doesn't handle assignment operators well, so when the mBrain vector gets 
	// resized, because it's an array, deep copy breaks the pointers. This is a bug and needs to be fixed
	// for now, the work around is to force the brain to reserve a high capacity so that the vector doesn't need to resize
	// and thus there will not be  deep copies when
	mBrain.reserve(2);

	// populate my state machine
	mBrain[0].AddState(new SM_Wait);
	mBrain[0].AddState(new SM_PickTarget);
	mBrain[0].AddState(new SM_GoToTarget);

	SM_FollowPath* superState = new SM_FollowPath();
	superState->mInternalStateMachine.AddState(new SM_OnPath);
	superState->mInternalStateMachine.SetInitState("On Path");
	mBrain[0].AddState(superState);

	// initial state
	mBrain[0].SetInitState("Wait");
	mBrain[0].Reset();

	// initialize the navigation state machine
	mBrain.push_back(StateMachine(this));
	SM_Navigate* navState = new SM_Navigate(&mBrain[0]);
	navState->mInternalStateMachine.AddState(new State("Idle")); // add dummy state
	navState->mInternalStateMachine.AddState(new SM_Nav_Active(&mBrain[0]));
	navState->mInternalStateMachine.SetInitState("Idle");
	mBrain[1].AddState(navState);
	mBrain[1].SetInitState("Navigate");
	mBrain[1].Reset();

	// Load the path
	std::ifstream fp("data/path.txt");
	if (fp.good()) {
		mPath.FromStream(fp);
		fp.close();
	}
}

void DemoActor::Update()
{
	// custom logic
	if (mBodyCollider.mbHasCollided) {
		// go to wait state.
		if(mBrain[0].mCurrentState && mBrain[0].mCurrentState->mName != "Pick Target")
			mBrain[0].ChangeState("Wait");
	}
		

	// IMPORTANT: Update the state machines
	Actor::Update();

	// After logic, update the body
	mBodyCollider.mPosition = mPosition;
	mBodyCollider.mRotation = mRotation;
	mBodyCollider.mScale = mScale;
}

void DemoActor::Render(AEMtx33* camMtx)
{
	// get position from collider
	// we know that render is called after the physics step
	// so the position of the collider might have been modified
	// due to collisions. 
	mPosition = mSensorCollider.mPosition = mBodyCollider.mPosition;

	// Render normally
	Actor::Render(camMtx);

	// debug drawing the path
	AEGfxSetTransform(&(*camMtx * AEMtx33::Translate(mPathPosition.x, mPathPosition.y)));
	DrawPath(mPath);

	// debug draw colliders
	mBodyCollider.Render(camMtx);
	mSensorCollider.Render(camMtx);
}

void DemoActor::InitializeColliders()
{
	mBodyCollider.mPosition = mPosition;
	mBodyCollider.mScale = mScale;
	mBodyCollider.mRotation = mRotation;
	mBodyCollider.mColor = AE_COLORS_BLACK;

	mBodyCollider.mCollisionShape = CSHAPE_OBB;
	gCollisionSystem.AddRigidBody(&mBodyCollider, true);

	mSensorCollider.mCollisionShape = CSHAPE_CIRCLE;
	mSensorCollider.mPosition = mPosition;
	mSensorCollider.mScale.x = 350.0f;
	mSensorCollider.mColor = AE_COLORS_BLACK;

}