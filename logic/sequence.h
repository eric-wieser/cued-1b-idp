#pragma once


#include "../robot.h"


class Sequence
{
protected:
	Robot& _robot;
public:
	Sequence(Robot& robot);
	~Sequence();

	void reset() {};
	bool end() const {return false;};

	virtual void step() = 0;
};


class StateMachine : public Sequence
{
protected:
	int _lastState;
	int _state;
	bool _transition;
	virtual void _step() = 0;
public:
	bool debug;

	StateMachine(Robot& robot);
	~StateMachine();

	void reset();
	bool end() const;
	void step();

	void run();

	template<class T>
	static int run();
};