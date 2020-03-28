/*!
*	\file		SpriteAnimation.h
*	\brief		Header for the SpriteAnimationData class
*	\details	SpriteAnimationData represents the resource for a sprite 
*				animation. It is meant to be used by the Sprite game object.
*
*	\remark		SpriteAnimationData is a resource, so it can be shared by 
*				multiple objects. 
*
*	\author		Thomas Komair - tkomair@digipen.edu
*	\date		12/02/2018
*	\copyright	Copyright DigiPen Institute Of Technology. All Rights Reserved
*
*/
#ifndef SPRITE_ANIM_DATA_H_
#define SPRITE_ANIM_DATA_H_

#include <AEEngine.h>

#define ANIM_ERR_DUPLICATE_NAME			0xFFFFFFFF
#define ANIM_ERR_ANIMATION_EXISTS		0xFFFFFFFE
#define ANIM_ERR_NO_ERROR				0

struct SpriteAnimationData
{
	std::string					mName;			// name of the animation
	std::vector<AEGfxTexture*>	mFrames;		// frame list
	std::vector<f32>			mDelays;		// Delays
	f32							mDuration;		// animation duration
	u32							mFrameCount;	// framecount
};

// Load from file
SpriteAnimationData *	SpriteAnimLoadFromFile(const char * filename);

// Save to file
void					SpriteAnimSaveToFile(const char * filename, SpriteAnimationData * animation);

// Add an animation 
int						SpriteAnimAddNew(SpriteAnimationData * animation);

// Get an animation
SpriteAnimationData *	SpriteAnimGetByName(const char * animName);

// Free All animations
void					SpriteAnimFreeAll();

// ---------------------------------------------------------------------------
// EXTRA CREDIT
struct Spritesheet
{
	std::string			mName;			// Name of the animation
	AEGfxTexture *		mTexture;		// Texture containing all the frames
	f32					mDuration;		// animation duration
	u32					mFrameCount;	// frame count
	u32					mRows;			// row count in the sprite sheet
	u32					mColumns;		// column count in the sprite sheet
	std::vector<f32>	mDelays;		// delays for each frame
};

// Load from file
Spritesheet *		Spritesheet_LoadFromFile(const char * filename);

// Save to file
void				Spritesheet_SaveToFile(const char * filename, Spritesheet * spritesheet);

// Add a new sprite sheet animation
int					Spritesheet_AddNew(Spritesheet * animation);

// Get a Spritesheet by name
Spritesheet*		Spritesheet_GetByName(const char* animName);

// Free all animations
void				Spritesheet_FreeAll();

// ---------------------------------------------------------------------------
#endif