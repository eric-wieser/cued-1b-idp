
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
	state(0);
}


bool StateMachine::end() const
{
	return state() == -1;
}


int StateMachine::state() const
{
	return _state;
}

void StateMachine::state(int st)
{
	_state = st;
	_transition = true;

	if (debug) {
		std::cout << "S" << st << std::endl;
	}
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