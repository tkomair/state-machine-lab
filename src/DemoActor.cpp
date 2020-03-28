#include <AEEngine.h>		// Alpha Engine
#include <GAM150.h>			// GAM 150 Labs
#include <Engine/Engine.h>		// CS 230 HW 4
#include <Demos/Common.h>		// CS 230 HW 4
#include <Demos/CurveEditor.h>  // CS 230 HW 4
#include "DemoActor.h"
#include <fstream>
#include <sstream>
#include <iomanip>
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
			mInternalStateMachine.ChangeState("Idle");

	}
};


struct SM_Nav_Active : public State {
	SM_Nav_Active(StateMachine* wanderSM) 
		:State("Active"), mWanderSM(wanderSM) {}

	StateMachine* mWanderSM = nullptr;
	AEVec2 mOriginalTarget; 
	AEVec2 mPreviousPosition;
	float mStuckTimer = 0.0f;
	void LogicEnter() {
		SM_GoToTarget* gotoState = (SM_GoToTarget*)mWanderSM->GetState("Go To Target");
		mOriginalTarget = gotoState->mTarget;
		mPreviousPosition = mActor->mPosition;
		mStuckTimer = 0.0f;
	}
	void LogicUpdate() {
		SM_GoToTarget* gotoState = (SM_GoToTarget*)mWanderSM->GetState("Go To Target");

		// use sensor to check surroundings
		DemoActor* actor = (DemoActor*)mActor;
		OBJECT_PTR_LIST	sensedObjects;
		gCollisionSystem.CollideBodyWithStatics(&actor->mSensorCollider, sensedObjects);


		// for debug draw below
		AEGfxSetTransform(&gCamera.GetWorldToCam());

		// navigation vector
		AEVec2 direction = (mOriginalTarget - actor->mPosition).Normalize() * 2;

		// loop through all the objects 
		for (auto obj : sensedObjects)
		{
			RigidBody* body = (RigidBody*)obj;

			// make a normalized ray
			Ray r; r.mOrigin = actor->mPosition;
			r.mDirection = (body->mPosition - r.mOrigin).Normalize();
			f32 t = -1.0f;
			Vector2 outP;
			// ray cast against these objects
			switch (body->mCollisionShape)
			{
			case CSHAPE_CIRCLE: 
				t = RayCastCircle(r, body->mPosition, body->mScale.x, &outP);
				break;
			default:
				break;
			}

			if (t > 0) {

				// since the ray is normalized, then t represents the distance to the object we're raycasting
				// assuming the max distance for seeing somethign is the radius of our collider, we're 
				// going to normalize t with respect to this value. 
				f32 distToObj = t; // store the distance to object
				t = t / actor->mSensorCollider.mScale.x;

				// depending on t, we're going to move away from the obstacle
				AEVec2 away = -r.mDirection * (1-t);
				if (t < 0.1)away *= 2;
				direction += away;

				// debug draw
				AEGfxRect(outP.x, outP.y, 0, 10, 10, AE_COLORS_BLUE);
				auto winP = gCamera.WorldPointToViewport(outP + AEVec2(20,20));
				winP.y = gAESysWinHeight - winP.y;
				std::stringstream str;
				str << std::setprecision(2) << t * 100.0f << "%";
				AEGfxPrint(winP.x, winP.y, AE_COLORS_BLUE, str.str().c_str());
			}


			// debug draw
			AEGfxLine(actor->mPosition.x, actor->mPosition.y, 0, AE_COLORS_BLUE,
				obj->mPosition.x, obj->mPosition.y, 0, AE_COLORS_BLUE);
		}
		

		// if we're in place
		f32 dist = actor->mPosition.Distance(mPreviousPosition);
		AEGfxPrint(400, 30, AE_COLORS_BLACK, std::to_string(dist).c_str());
		if (dist < 10) {
			mStuckTimer += gAEFrameTime;
			if (mStuckTimer > 2.0f) {
				mWanderSM->ChangeState("Wait"); // this will change our state internally. 
			}

		}
		else mStuckTimer = 0.0f;
		mPreviousPosition = actor->mPosition;

		// set the target ahead of the player
		gotoState->mTarget = actor->mPosition + direction * actor->mMoveSpeed;

		// debug-> let the player take over
		if (AEInputMousePressed(AE_MOUSE_LEFT)) {
			gotoState->mTarget = gCamera.WindowPointToWorld(gAEMousePosition);
		}

		// if we've reached the original target wer're done
		if (actor->mPosition.Distance(mOriginalTarget) < 15)
			mWanderSM->ChangeState("Wait"); // this will change our state internally. 

		// draw to original target
		AEGfxLine(actor->mPosition.x, actor->mPosition.y, 0, AE_COLORS_BROWN,
			mOriginalTarget.x, mOriginalTarget.y, 0, AE_COLORS_BROWN);
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
		if (mTimeInCollision > 2.0f && mBrain[0].mCurrentState && mBrain[0].mCurrentState->mName != "Pick Target") {
			mTimeInCollision = 0.0f;
			mBrain[0].ChangeState("Wait");
		}

		// increase the time in collision
		mTimeInCollision += gAEFrameTime;
	}
	else mTimeInCollision = 0.0f;
		

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
	mSensorCollider.mScale.x = 550.0f;
	mSensorCollider.mColor = AE_COLORS_BLUE;

}