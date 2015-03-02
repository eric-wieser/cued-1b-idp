
#include <iostream>
#include "../robot.h"

#include "sequence.h"


Sequence::Sequence(Robot& robot)
		: _robot(robot)
{
}


Sequence::~Sequence()
{
}



StateMachine::StateMachine(Robot& robot)
		: Sequence(robot), _state(0), debug(false)
{
}


StateMachine::~StateMachine()
{
}


void StateMachine::reset()
{
	_state = 0;
}


bool StateMachine::end() const
{
	return _state == -1;
}


void StateMachine::step()
{
	// if we're entering a new state, debug
	_transition = _state != _lastState;
	if(debug && _transition) {
		std::cout << "S" << _state << std::endl;
	}

	// store the current state so that _lastState is valid inside _step()
	int last = _state;
	_step();
	_lastState = last;
}


void StateMachine::run()
{
	do
	{
		this->operator()();
	} while (!end());
}


template<class T>
int StateMachine::run()
{
	T sm;

	do
	{
		sm();
	} while (!sm.end());

	return sm.state();
}