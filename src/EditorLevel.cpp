// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	Level.cpp
// Author			:	Thomas Komair
// Creation Date	:	2017/03/27
// Purpose			:	Implementation of demo level game states functions.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// INCLUDES
#include <AEEngine.h>
#include <GAM150.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <Engine\Engine.h>
#include <Engine\Animation\Path2D.h>
#include <Demos/Common.h>
#include <Demos/CurveEditor.h>
#include "labCommon.h"
#include "EditorLevel.h"

using std::cout;
using std::endl;
// ---------------------------------------------------------------------------
// GLOBAL VARIABLES AND DEFINITIONS

namespace EditorLevel
{
	// global path we're manipulating
	Path2D gPath;
	bool gbShowPointInfo = false;
	bool gbAutoPlay = true;
	bool gbLoop = true;

	// main object to show how 
	// path is moving with time
	GameObject * gpObj;
	f32 gTimer = 0.0f;

	// Manual testing of curves.
	void DefaultPath();
}
using namespace EditorLevel;

// ---------------------------------------------------------------------------
// GAME STATE FUNCTIONS
void EditorLevel_Load()
{
	gpCircle = MakeCircle(50, CommonColors::Black);
	CamInit();
}
void EditorLevel_Init()
{
	AEGfxSetClearColor(CommonColors::White);
	gTimer = 0.0f;
	gPickedPoint = -1;

	gpObj = gObjMgr.AddObject(new GameObject, "circle", 0, 0, 25, 25, 0, gpCircle);
	gpObj->mbUseModColor = true;
	gpObj->mColor = AEGfxColor(188, 40, 70, 128);

	DefaultPath();
}
void EditorLevel_Update()
{
	labSM::GameFlow();
	gObjMgr.Update();
	CamUpdate();
	EditPath(gPath);

	// global control
	if (AEInputKeyTriggered(VK_RETURN)){
		gbAutoPlay = !gbAutoPlay;
	}
	if (AEInputKeyTriggered('D')){
		gbShowPointInfo = !gbShowPointInfo;
	}
	if (AEInputKeyTriggered('L')){
		gbLoop = !gbLoop;
	}

	// animation time control
	if (gPath.Size()){
		f32 timeStep = 0.01667f;
		if (AEInputKeyTriggered(VK_SPACE)){
			gTimer = 0.0f;
		}
		if (AEInputKeyPressed(VK_LEFT)){
			gTimer -= timeStep;
			gbAutoPlay = false;
		}
		if (AEInputKeyPressed(VK_RIGHT)){
			gTimer += timeStep;
			gbAutoPlay = false;
		}
		if (gbAutoPlay)
			gTimer += timeStep;// f32(gAEFrameTime);

		f32 animLength = gPath.Duration();
		if (gbLoop && gTimer >= animLength)
			gTimer -= animLength;

		gTimer = AEClamp(gTimer, 0, animLength);
	}

	// Save/Load
	if (AEInputKeyPressed(AE_KEY_CTRL) && AEInputKeyTriggered('S'))
		SavePathToFile(gPath);
	// Save/Load
	if (AEInputKeyPressed(AE_KEY_CTRL) && AEInputKeyTriggered('O'))
		LoadPathFromFile(gPath);

	Vector2 pos = gPath.SampleTime(gTimer);
	gpObj->mPosition = pos;
}
void EditorLevel_Render()
{
	static char buffer[512];
	sprintf_s(buffer,"Show point info (D) : %s\n\
Autoplay(ENTER) : %s\n\
Loop (L) : %s\n\
Reset time (SPACE)\n\n\
Animation Timer = %.2f\n\
Click and drag to move control points\n\
(+ CTRL to adjust time)\n\
(+ SHIFT apply to remaining ascending points)", 
					 gbShowPointInfo ? "TRUE" : "FALSE",
					 gbAutoPlay ? "TRUE" : "FALSE",
					 gbLoop ? "TRUE" : "FALSE", 
					 gTimer);
	AEGfxPrint(10, 10, CommonColors::Black, buffer);
	
	// compute world to cam matrix
	AEMtx33 w2c = gCamera.GetWorldToCam();

	// render all game objects
	gObjMgr.Render(&w2c);

	// custom rendering
	AEGfxReset();
	AEGfxSetTransform(&w2c);
	DrawPath(gPath);
	AEGfxFlush();

	// draw text for where the keyframe is
	if (gbShowPointInfo)
		DrawAllPointInfo(gPath);
	if (gPickedPoint != -1)
		DrawPointInfo(gPath, gPickedPoint);

}
void EditorLevel_Free()
{
	gPath.Clear();
	gObjMgr.FreeAllObjects();
}
void EditorLevel_Unload()
{
	AEGfxTriFree(gpCircle);
}

// ---------------------------------------------------------------------------
// HELPER FUNCTIONS IMPLEMENTATIONS
namespace EditorLevel
{
	void DefaultPath()
	{
		gPath.Clear();
		gPath.Push(0, Vector2(0, 0));
		gPath.Push(0.33f, Vector2(25, 100));
		gPath.Push(0.66f, Vector2(50, 100));
		gPath.Push(1.0f, Vector2(75, 0)); // common point
		gPath.Push(1.33f, Vector2(100, -100));
		gPath.Push(1.66f, Vector2(125, -100));
		gPath.Push(2, Vector2(150, 0));
	}
}