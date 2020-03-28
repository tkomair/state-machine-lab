#include <AEEngine.h>
#include <GAM150.h>
#include <Engine/Engine.h>
#include <Demos/Common.h>
#include "labCommon.h"

namespace testCollisionExtras {
	Ray r;
	RigidBody tempBody;
}using namespace testCollisionExtras;

void TestExtras_Load()
{
}
void TestExtras_Unload()
{
}
void TestExtras_Init()
{
	CamInit();
	labSM::MakeRandomLevel();
	tempBody.mScale = { 40, 90 };
	tempBody.mRotation = AERandFloat(0, TWO_PI);
	tempBody.mCollisionShape = CSHAPE_OBB;
	tempBody.mColor = AE_COLORS_BLUE;

}
void TestExtras_Free()
{
	gObjMgr.FreeAllObjects();
	gCollisionSystem.ClearBodies();
}
void TestExtras_Update()
{
	CamUpdate();
	gObjMgr.Update();
	gCollisionSystem.Update();

	if (AEInputMouseTriggered(0)) {
		r.mOrigin = gCamera.WindowPointToWorld(gAEMousePosition);
	}
	if (AEInputMousePressed(0)) {
		r.mDirection = gCamera.WindowPointToWorld(gAEMousePosition) - r.mOrigin;
	}
	if (AEInputMousePressed(1)) {
		tempBody.mPosition = gCamera.WindowPointToWorld(gAEMousePosition);
	}
	labSM::GameFlow();
}
void TestExtras_Render()
{
	AEGfxSetClearColor(AE_COLORS_WHITE);
	AEGfxClearBuffer();

	gObjMgr.Render(&gCamera.GetWorldToCam());
	// draw ray
	{
		Vector2 endRay = r.mOrigin + r.mDirection;
		AEGfxReset();
		AEGfxSetTransform(&gCamera.GetWorldToCam());
		AEGfxLine(r.mOrigin.x, r.mOrigin.y, 0, AE_COLORS_BLACK, endRay.x, endRay.y, 0, AE_COLORS_BLACK);

	}

	// raycast 
	{
		CollisionSystem::RaycastResult::LIST outList;
		if (-1.0f != gCollisionSystem.RaycastWithStatics(r, outList)) {
			u32 i = 0;
			for (auto it : outList) {
				if (it.mImpactTime < 1.0f) {
					AEGfxRect(it.mImpactPoint.x, it.mImpactPoint.y, 0, 10, 10, AE_COLORS_RED);
					auto ptWin = gCamera.WorldPointToViewport(it.mImpactPoint);
					ptWin.y = gAESysWinHeight - ptWin.y;
					AEGfxPrint((s32)ptWin.x, (s32)ptWin.y, AE_COLORS_BLACK, std::to_string(i).c_str());
					++i;
				}
			}
		}
	}
	AEGfxFlush();

	// collision test
	{
		tempBody.Render(&gCamera.GetWorldToCam());
		AEGfxSetTransform(&gCamera.GetWorldToCam());
		OBJECT_PTR_LIST outList;
		std::list<Contact> outContacts;
		if (gCollisionSystem.CollideBodyWithStatics(&tempBody, outList, &outContacts))
		{
			tempBody.mColor = AE_COLORS_RED;
			for (auto obj : outList)
				obj->mColor = AE_COLORS_RED;
			for (auto ct : outContacts) {
				AEGfxRect(ct.mPi.x, ct.mPi.y, 0, 10, 10, AE_COLORS_RED);
			}
		}
		else
			tempBody.mColor = AE_COLORS_BLUE;
	}
}