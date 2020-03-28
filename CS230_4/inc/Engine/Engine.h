// ----------------------------------------------------------------------------
// Project Name		:	Project 4
// File Name		:	Engine.h
// Author			:	Thomas Komair
// Creation Date	:	February 21st, 2013
// Purpose			:	Defines the engine headers.
// History			:
// - 2013/2/21		:	Initial Implementation.
// ----------------------------------------------------------------------------
#ifndef ENGINE_H_
#define ENGINE_H_

#include "GSM\GameStateManager.h"
#include "Math\MyMath.h"
#include "Physics\RigidBody.h"
#include "Physics\CollisionSystem.h"
#include "Animation\Easing.h"
#include "Animation\Path2D.h"
#include "AI\StateMachine.h"

#define FOR_EACH(itName, container) for (auto itName = container.begin(); itName != container.end(); ++itName)

#endif