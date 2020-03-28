// Lab - State Machines.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <AEEngine.h>       // Alpha Engine
#include <GAM150.h>			// GAM 150 Labs
#include <Engine/Engine.h>  // CS 230 Engine
#include "EditorLevel.h"
#include "labCommon.h"

int main()
{
	// Initialize the alpha engine
	if (AESysInit("Lab 4 - State Machines", 1280, 720) == false)
	{
		return 0;
	}

	// Add a game state to the game state manager
	AEGameStateMgrAdd(
		GS_EDITOR_LEVEL,
		EditorLevel_Load,
		EditorLevel_Init,
		EditorLevel_Update,
		EditorLevel_Render,
		EditorLevel_Free,
		EditorLevel_Unload);
	AEGameStateMgrAdd(
		GS_TEST_EXTRAS,
		TestExtras_Load,
		TestExtras_Init,
		TestExtras_Update,
		TestExtras_Render,
		TestExtras_Free,
		TestExtras_Unload);
	AEGameStateMgrAdd(
		GS_LAB_SM,
		Demo_Load,
		Demo_Init,
		Demo_Update,
		Demo_Render,
		Demo_Free,
		Demo_Unload);
	

	// Initialize the game state manager
	AEGameStateMgrInit(GS_LAB_SM);

	// Initialize the Object Manager
	gObjMgr.Initialize();

	// Initialize the collision system
	gCollisionSystem.Init();

	// Game Loop
	AESysGameLoop();

	// shutdown the collision system
	gCollisionSystem.Shutdown();

	gObjMgr.Shutdown();

	AESysExit();
}