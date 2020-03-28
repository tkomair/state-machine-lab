// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		Collisions.h
//	Purpose:		Declares static containment and overlap tests between 
//					different primitives.
//	Project:		cs230_tkomair_2_2
//	Author:			Thomas Komair (tkomair@digipen.edu)
// ----------------------------------------------------------------------------
#ifndef COLLISION_H
#define COLLISION_H
// ----------------------------------------------------------------------------

struct Vector2;

//! ---------------------------------------------------------------------------
// \fn		StaticPointToStaticCircle
// \brief	This function determines if a static point is inside a static circle
// 			- P:		The point to test for circle containment
// 			- Center:	Center of the circle
// 			- Radius:	Radius of the circle
// 
//  \return	true if the point is contained in the circle, false otherwise.
// ---------------------------------------------------------------------------
bool StaticPointToStaticCircle(Vector2 *P, Vector2 *Center, float Radius);

//! ---------------------------------------------------------------------------
//	\fn		StaticPointToStaticRect
//	\brief	This function checks if the point Pos is colliding with the rectangle
//			whose center is Rect, width is "Width" and height is Height
//
//  \return	true if the point is contained in the rectangle, false otherwise.
// ---------------------------------------------------------------------------
bool StaticPointToStaticRect(Vector2 *Pos, Vector2 *Rect, float Width, float Height);

//! ---------------------------------------------------------------------------
//	\fn		StaticPointToOrientedRect
//	\brief	This function checks if the point Pos is colliding with an oriented rectangle
//			whose center is Rect, width is "Width", height is Height and rotation "AngleRad"
//
//  \return	true if the point is contained in the rectangle, false otherwise.
// ---------------------------------------------------------------------------
bool StaticPointToOrientedRect(Vector2 *Pos, Vector2 *Rect, float Width, float Height, float AngleRad);

//! ---------------------------------------------------------------------------
//	\fn		StaticPointToStaticCircle
//	\brief	This function checks for collision between 2 circles.
//			Circle0: Center is Center0, radius is "Radius0"
//			Circle1: Center is Center1, radius is "Radius1"
//
//  \return	true if both circles overlap, false otherwise.
// ---------------------------------------------------------------------------
bool StaticCircleToStaticCircle(Vector2 *Center0, float Radius0, Vector2 *Center1, float Radius1);

//! ---------------------------------------------------------------------------
// \fn		StaticRectToStaticRect
// \brief	This functions checks if 2 rectangles are colliding
//			Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
//			Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
//
//  \return	true if both rectangles overlap, false otherwise.
// ---------------------------------------------------------------------------
bool StaticRectToStaticRect(Vector2 *Rect0, float Width0, float Height0, Vector2 *Rect1, float Width1, float Height1);

//! ---------------------------------------------------------------------------
// \fn		StaticRectToStaticCirlce
// \brief	This function checks if a circle is colliding with a rectangle
//			Rectangle: Defined at center "Rect" of width and height "Width", "Height", respectively
//			Center: Defined at center "Center", and of radius "Radius"
//
//  \return	true if both shapes overlap, false otherwise.
// ---------------------------------------------------------------------------
bool StaticRectToStaticCirlce(Vector2 * Rect, float Width, float Height, Vector2* Center, float Radius);

//! ---------------------------------------------------------------------------
// \fn		OrientedRectToStaticCirlce
// \brief	This function checks if a circle is colliding with an oriented rectangle
//			Rectangle: Defined at center "Rect" of width and height "Width", "Height", 
//			and rotation "AngelRad" respectively
//			Center: Defined at center "Center", and of radius "Radius"
//
//  \return	true if both shapes overlap, false otherwise.
// ---------------------------------------------------------------------------
bool OrientedRectToStaticCirlce(Vector2 * Rect, float Width, float Height, float AngleRad, Vector2* Center, float Radius);


#endif
