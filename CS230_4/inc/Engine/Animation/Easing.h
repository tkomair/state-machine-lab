// ----------------------------------------------------------------------------
#ifndef CS230_EASING_H_
#define CS230_EASING_H_
// ----------------------------------------------------------------------------

//! Typedef for tween function
typedef f32(*EaseFunc)(f32);


// ----------------------------------------------------------------------------
// @TODO
/*!
\brief	Given an input value between [0,1], returns eased value using:
		linear easing (no effect).
\return	New time value.
*/
// ---------------------------------------------------------------------------
f32 EaseLinear(f32 tn);

// @TODO
//! ---------------------------------------------------------------------------
// \fn		EaseInQuad
// \brief	Given an input value between [0,1], returns eased value using:
//			Ease in: Accelerate towards goal.
// \return	new time value.
// ---------------------------------------------------------------------------
f32 EaseInQuad(f32 tn);

// @TODO
//! ---------------------------------------------------------------------------
// \fn		EaseOutQuad
// \brief	Given an input value between [0,1], returns eased value using:
//			Ease Out: Decelerate towards goal.
// \return	new time value.
// ---------------------------------------------------------------------------
f32 EaseOutQuad(f32 tn);

// @TODO
//! ---------------------------------------------------------------------------
// \fn		EaseInOutQuad
// \brief	Given an input value between [0,1], returns eased value using:
//			Ease In/Out: EaseIn from tn = [0,0.5], EaseOut from tn [0.5,1]
// \return	new time value.
// ---------------------------------------------------------------------------
f32 EaseInOutQuad(f32 tn);


#pragma endregion

// ----------------------------------------------------------------------------
#endif