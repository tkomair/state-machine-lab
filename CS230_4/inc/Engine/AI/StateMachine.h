#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <SpriteAnimation.h>
#include <Sprite.h>

// forward declare the GameObject structure
// This is so that we don't have to include it in the header
// remember that this is only valid if we reference that struct
// by pointer. 
struct Actor;
struct StateMachine;

//@TODO
//! \class State
//! \brief Represents a state in a state machine. This class is meant to be overwritten. 
struct State
{
	// ------------------------------------------------------------------------
	// DATA

	//! Name of the state.
	std::string mName;

	//! Pointer to the state machine that owns this state.
	StateMachine * mOwnerStateMachine;

	//! Pointer to the game object that owns the state machine.
	Actor * mActor;

	//! Specifies how long this state has been active.
	f32 mTimeInState;

	// ------------------------------------------------------------------------
	// METHODS

	// @TODO
	//! Sets name to specified name
	State(const char * name);

	// @PROVIDED
	// !Destructor. Doesn't do anything, needed for inherited classes
	virtual ~State() {}

	// @PROVIDED
	//! Default Enter method. Does nothing as it is meant to be overriden by derived classes. 
	virtual void LogicEnter(){}

	// @PROVIDED
	//! Default Exit method. Does nothing as it is meant to be overriden by derived classes. 
	virtual void LogicExit(){}

	// @PROVIDED
	//! Default Update method. Does nothing as it is meant to be overriden by derived classes. 
	virtual void LogicUpdate(){}

	// @TODO
	//! Internal version of Enter called by state machine.
	virtual void InternalEnter();

	// @TODO
	//! Internal version of Exit called by state machine.
	virtual void InternalExit();

	// @TODO
	//! Internal version of Update called by state machine.
	virtual void InternalUpdate();
};

// @TODO
//! \class StateMachine
//! \brief Represents a finite state machine used for implementing complex behaviors.
struct StateMachine
{
	// ------------------------------------------------------------------------
	// DATA

	//! State container. This class owns the memory for the states and thus should free it when destroyed. 
	std::vector<State*> mStates;

	//! Pointer to the game object that owns the state machine.
	Actor * mActor;

	//! Pointer to the current active state in the state machine. 
	State * mCurrentState;

	//! Pointer to the next active state. Same as mCurrentState unless state change is requested. 
	State * mNextState;

	//! Pointer to the initial state of the state machine. This is the active state when the state  machine is reset. 
	State * mInitialState;

	// ------------------------------------------------------------------------
	// METHODS

	// @TODO
	//! \brief	Sets all state machine variables to NULL (note: they are all pointers).
	StateMachine();

	// @TODO
	//! \brief	Sets the actor and the rest of variables to NULL (note: they are all pointers).
	StateMachine(Actor * actor);

	// @TODO
	//! Clears all the states (hint: calls Clear function below)
	~StateMachine();
	
	//
	// CONTAINER MANAGEMENT
	//

	// @TODO
	//! Adds a new state to the state machine.
	void AddState(State * state);

	// @TODO
	//! Removes a State from the state machine.
	void RemoveState(State * state);

	// @TODO
	//! Removes all the states from the state machine and sets the state pointers (current, next, initial) to NULL.
	void Clear();

	// @TODO
	//! Finds a state by name
	State* GetState(const char * stateName);

	// @TODO
	//! Sets the initial state to the given pointer.
	void SetInitState(State * state);

	// @TODO
	//! Sets the initial state to one state whose name matches the specified one.
	void SetInitState(const char * stateName);

	// @TODO
	//! Changes state to the specified state. 
	void ChangeState(State * state);

	// @TODO
	//! Changes state to the specified state by name. 
	void ChangeState(const char * stateName);

	// @TODO
	//! Update of the state machine. Meant to be called every frame.
	void Update();

	// @TODO
	//! Resets the state machine. 
	void Reset();
};

// @TODO
//! \class SuperState
//! \brief State that acts as a state machine. Allows nested state machines.
struct SuperState : public State
{
	// ------------------------------------------------------------------------
	// DATA

	//! Internal state machine. InternalUpdate calls Update
	StateMachine mInternalStateMachine;

	// ------------------------------------------------------------------------
	// METHODS

	// @TODO
	//! Sets the name and actor of the super state
	SuperState(const char * name, Actor * actor);

	// @TODO
	//! Resets the internal state machine and calls the base InternalEnter.
	virtual void InternalEnter();

	// @TODO
	//! Updates the internal state machine and calls the base InternalUpdate();
	virtual void InternalUpdate();
};

// @PROVIDED
//! \class Actor
//! \brief Simple GameObject structure that supports state machines. 
//! \details Feel free to use this structure in your demos.  
struct Actor : public Sprite
{
	//! Use a vector to support concurrent state machine
	std::vector<StateMachine> mBrain;

	//! Adds one state machine to be used by default.
	Actor() :Sprite()
	{
		// add default state machine
		mBrain.push_back(StateMachine(this));
	}

	//! Updates all state machine sequentially.
	virtual void Update()
	{
		for (u32 i = 0; i < mBrain.size(); ++i)
			mBrain[i].Update();

		Sprite::Update();
	}

	// @SUGGESTION
	/*! Add extra functionality to easily manage the actor's state machines
		and their respective states. For example, the following functions could be 
		useful:

		State * GetState(stateMachineName, stateName);
		State * AddState(stateMachineName, newState);
		void  ChangeState(stateMachineName, nextState);

		State * AddState(newState); // adds to default state machine.
		State * GetState(stateName); // gets state from default state machine.
		boid ChangeState(nextState); // changes state in default state machine. 

		etc...
	*/
};

// @PROVIDED
//! \class	AnimState
//! \brief	Basic state that overrides the internal enter to automatically 
//!			change the animation on a Sprite object. 
//! \details Feel free to use this structure in your demos.
struct AnimState : public State
{
	Spritesheet	*			mSpriteSheet;
	SpriteAnimationData *	mAnimation;
	u32						mInitialFrame;
	f32						mAnimSpeed;
	bool					mLoop;

	AnimState(const char * name)
		: State(name)
		, mSpriteSheet(0)
		, mAnimation(0)
		, mInitialFrame(0)
		, mAnimSpeed(1.0f)
		, mLoop(false)
	{}

	// internal versions
	void InternalEnter()
	{
		if (mAnimation) {
			// set the animation state data
			Sprite * sprite = (Sprite*)mActor;
			sprite->SetAnimation(sprite->mpAnimData);
			sprite->SetLoop(mLoop);
			sprite->mAnimSpeed = mAnimSpeed;
			sprite->SetAnimationFrame(mInitialFrame);
		}
		else if (mSpriteSheet){
			// set animation state
			Sprite * sprite = (Sprite *)mActor;
			sprite->SetSpritesheet(mSpriteSheet);
			sprite->SetLoop(mLoop);
			sprite->mAnimSpeed = mAnimSpeed;
			sprite->SetAnimationFrame(mInitialFrame);
		}
		mActor->mbAnimPlaying = true;

		// always call the base function to insure default functionality
		State::InternalEnter();
	}
};

#endif