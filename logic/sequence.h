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
	int state() const {return 0;};

	virtual void operator()() = 0;
};


class StateMachine : public Sequence
{
protected:
	int _state;
	bool _transition;
public:
	bool debug;

	StateMachine(Robot& robot);
	~StateMachine();
	
	void reset();
	bool end() const;
	int state() const;
	void state(int st);

	virtual void operator()() = 0;

	void run();

	template<class T>
	static int run();
};