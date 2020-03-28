#include <AEEngine.h>
#include <GAM150.h>
#include <Engine/Engine.h>
#include <Demos/Common.h>
#include "EditorLevel.h"
#include "labCommon.h"
#include "DemoActor.h"

namespace labSM {
	AEGfxTriList* gQuad = NULL;
}using namespace labSM;

void Demo_Load()
{
	labSM::gQuad = MakeQuad(AE_COLORS_WHITE);
}
void Demo_Unload()
{
	AEGfxTriFree(gQuad);
}
void Demo_Init()
{
	MakeRandomLevel2(150,{ 2500,2500}, {10000,10000});
	CamInit();
	
	// add actor
	{
		auto obj = gObjMgr.AddObject(new DemoActor, "Actor", 0, 0, 50, 50, 0, gQuad);
		obj->mbUseModColor = true;
		obj->mColor = AEGfxColor(0, 0, 0, 128);
	// add cam controller
		auto cam = gObjMgr.AddObject(new DemoCamera(&gCamera, obj), "CamController");
	}
}
void Demo_Free()
{
	gObjMgr.FreeAllObjects();
	gCollisionSystem.ClearBodies();
}
void Demo_Update()
{
	CamUpdate();
	gObjMgr.Update();
	gCollisionSystem.Update();
	DebugStateMachine();

	labSM::GameFlow();
}
void Demo_Render() 
{
	AEGfxSetClearColor(AE_COLORS_WHITE);
	AEGfxClearBuffer();

	gObjMgr.Render(&gCamera.GetWorldToCam());
	AEGfxSetTransform(&gCamera.GetWorldToCam());
}

namespace labSM {
	void GameFlow()
	{
		if (AEInputKeyPressed(AE_KEY_CTRL) && AEInputKeyTriggered('R'))
			gAEGameStateNext = AE_GS_RESTART;
		if (AEInputKeyPressed(AE_KEY_CTRL) && AEInputKeyTriggered('Q'))
			gAEGameStateNext = AE_GS_QUIT;

		if (AEInputKeyTriggered('1'))
			gAEGameStateNext = GS_LAB_SM;
		if (AEInputKeyTriggered('2'))
			gAEGameStateNext = GS_TEST_EXTRAS;
		if (AEInputKeyTriggered('3'))
			gAEGameStateNext = GS_EDITOR_LEVEL;
	}
	void MakeRandomLevel()
	{
		//  create a static rigid body and add to the collision system

		f32 posX = 0;
		for (u32 i = 0; i < 100; ++i)
		{
			RigidBody* body = nullptr;
			if (rand() % 8)
				body = CreateRandBoxWithMass(0, Vector2());
			else
				body = CreateRandCircleWithMass(0, Vector2());

			body->mName = "Level";
			body->mPosition = AEVec2(posX, AERandFloat(-100, 100));
			if (i == 0)
				body->mPosition = AEVec2(posX, -100);

			body->mScale = AEVec2(AERandFloat(400, 1500), 50);
			body->mRotation = AERandFloat(-PI / 5, PI / 5);
			body->mColor = 0xFF000000;

			posX += body->mScale.x / 2 + AERandFloat(100, 250);
		}
	}
	void MakeRandomLevel2(u32 count, AEVec2 worldMin, AEVec2 worldMax) {

		AEVec2 WMin = worldMin, WMax = worldMax; 
		f32 stepParams[] = {0.1f, 0.05f, 0.2f, 0.15f, 0.2f, 0.1f};
		u32 steps = sizeof(stepParams) / sizeof(f32);
		for (u32 s = 0; s < steps; ++s)
		{
			worldMin = WMin * (f32)(s + 1) / (f32)steps;
			worldMax = WMax * (f32)(s + 1) / (f32)steps;

			u32 toMake = u32(stepParams[s] * count);
			for (u32 i = 0; i < toMake; ++i)
			{
				RigidBody* body = nullptr;
				if (rand() % 2 == 3) {
					body = CreateRandBoxWithMass(0, Vector2());
					body->mScale = AEVec2::Random(200, 400, 20, 100);
					body->mRotation = AERandFloat(-PI / 5, PI / 5);
				}
				else {
					body = CreateRandCircleWithMass(0, Vector2());
					body->mScale = AEVec2(AERandFloat(50, 250), 1);
				}

				body->mName = "Level";
				//body->mPosition = AEVec2::Random(minTmp.x, maxTmp.x, minTmp.y, maxTmp.y);
				if (rand() % 2) { // free on Y
					body->mPosition.x = rand() % 2 ? AERandFloat(-worldMax.x * 0.5f, -worldMin.x * 0.5f) : AERandFloat(worldMin.x * 0.5f, worldMax.x * 0.5f);
					body->mPosition.y = AERandFloat(-worldMax.y * 0.5f, worldMax.y * 0.5f);
				}
				else {
					body->mPosition.x = AERandFloat(-worldMax.x * 0.5f, worldMax.x * 0.5f);
					body->mPosition.y = rand() % 2 ? AERandFloat(-worldMax.y * 0.5f, -worldMin.y * 0.5f) : AERandFloat(worldMin.y * 0.5f, worldMax.y * 0.5f);

				}
				body->mColor = AE_COLORS_BLACK;
			}
		}
	}
}