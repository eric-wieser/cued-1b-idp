
#include <iostream>
#include "../robot.h"
#include "../timing.h"

#include "sequence.h"


Sequence::Sequence(Robot& robot)
		: _robot(robot)
{
}


Sequence::~Sequence()
{
}



StateMachine::StateMachine(Robot& robot)
		: Sequence(robot), lastState(-1), _state(0), _timer(0), debug(false)
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
	if (_transition) {
		if (debug) {
			std::cout << "S" << _state << std::endl;
		}

		_timer = timing::now();
	}

	// store the current state so that _lastState is valid inside _step()
	int last = _state;
	_step();
	_lastState = last;
}


double stateTime() const {
	return timing::diff(_timer);
}


void StateMachine::run()
{
	do
	{
		this->step();
	} while (!end());
}


template<class T>
int StateMachine::run()
{
	T sm;

	do
	{
		sm.step();
	} while (!sm.end());

	return sm.state();
}