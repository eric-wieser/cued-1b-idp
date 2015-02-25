
#pragma once

#include "timing.h"
#include "robot.h"


class LineFinder
{
private:
	Robot& _robot;
	int _state;
	int _nLine;
	int _nInvalid;
	t_timer _timer;

public:
	LineFinder(Robot& robot);
	void reset();
	bool found() const;
	bool completelyLost() const;

	void operator()();

	static bool run(Robot& robot);
};


class LineFollow
{
private:
	Robot& _robot;
	int _state;
	int _nJunc;
	int _nNone;
	int _nInvalid;
	int _nLine;
	t_timer _timer;
	LineFinder _lineFinder;

public:
	LineFollow(Robot& robot);

	bool junction() const;
	void reset();

	void operator()();

	static void gotoJunction(Robot& robot);
};