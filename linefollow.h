
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
	enum State{
		BEGIN = 0,
		SWEEP_LEFT = 1,
		SWEEP_RIGHT = 2,

		STILL_LOST = 20,
		LINE_FOUND = 30
	};
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
	enum State{
		BEGIN = 0,
		ON_LINE = 1,
		LINE_LOST = 20,
		LINE_LOST1 = 21,

		JUNCTION = 30,

		TOTALLY_LOST = 40
	};
	LineFollow(Robot& robot);

	bool junction() const;
	void reset();

	void operator()();

	static void gotoJunction(Robot& robot);
};