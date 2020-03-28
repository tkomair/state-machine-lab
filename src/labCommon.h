#pragma once

#define GS_LAB_SM 123
#define GS_TEST_EXTRAS 124

void Demo_Load();
void Demo_Unload();
void Demo_Init();
void Demo_Free();
void Demo_Update();
void Demo_Render();

void TestExtras_Load();
void TestExtras_Unload();
void TestExtras_Init();
void TestExtras_Free();
void TestExtras_Update();
void TestExtras_Render();

namespace labSM
{
	void GameFlow();
	void MakeRandomLevel();
	void MakeRandomLevel2(u32 count, AEVec2 worldMin, AEVec2 worldMax);
}