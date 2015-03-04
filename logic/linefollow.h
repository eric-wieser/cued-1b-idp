
#pragma once

#include "util/timing.h"
#include "robot.h"
#include "sequence.h"

class LineFinder : public StateMachine
{
private:
	int _nLine;
	int _nInvalid;

	void _step();

public:
	enum State{
		BEGIN = 0,
		SWEEP_LEFT1 = 1,
		SWEEP_RIGHT1 = 2,
		SWEEP_RIGHT2 = 3,
		SWEEP_LEFT2 = 4,

		STILL_LOST = 20,
		LINE_FOUND = 30
	};

	LineFinder(Robot& robot);
	bool found() const;
	bool completelyLost() const;
	bool end() const;
};


class LineFollow : public StateMachine
{
private:
	int _nJunc;
	int _nNone;
	int _nInvalid;
	int _nLine;
	LineFinder _lineFinder;

	void _step();

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
	bool end() const;
};