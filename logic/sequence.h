#pragma once

#include "../timing.h"
#include "../robot.h"


/** Abstract sequence base class
Sequences are typically implemented procuderal code or state machines.
*/
class Sequence
{
protected:
	Robot& _robot;
public:
	Sequence(Robot& robot);
	~Sequence();

	/** Runs the current sequence
	*/
	virtual void run() = 0;
};


/** Base class for state machines
Handles common operations used by a state machine.
StateMachine implementations should override the virtual _step method
*/
class StateMachine : public Sequence
{
protected:
	int _lastState;
	int _state;
	bool _transition;
	t_timer _timer;

	/** State machine logic implementation method.
	*/
	virtual void _step() = 0;
public:
	bool debug;

	StateMachine(Robot& robot);
	~StateMachine();

	/** Resets the machine to its initial state
	Should be overriden to clear any other state variables. Implementations should
	prefer to destruct and reconstruct a new state machine.
	*/
	void reset();

	/** Checks whether the state machine is in an end state.
	By default this is state -1, but this method should be
	overriden to check for all cases
	@return True if the machine is in an end state.
	*/
	bool end() const;

	/** Steps through 1 iteration of the machine
	This performs some background logic internally, but
	is essentially a wrapper around the _step method.
	*/
	void step();

	/** Returns the number of milliseconds the machine has been in its current state.
	@return Milliseconds since state started.
	*/
	double stateTime() const;

	/** Runs the current instance of machine until an end state
	*/
	void run();

	/** Runs a state machine from beginning to an end state.
	@return The state number the machine ended in.
	*/
	template<class T>
	static int run();
};