
#include <stdexcept>
#include "util/signals.h"
#include "timing.h"
#include "robot.h"

#include "linefollow.h"
#include "util/signals.h"

LineFollow::LineFollow(Robot& robot)
		: _robot(robot), _state(0), _nJunc(0), _nNone(0), _nInvalid(0), _nLine(0), _timer(0), _lineFinder(robot)
{
}


bool LineFollow::junction() const
{
	return _state == 30;
}


void LineFollow::reset()
{
	_state = 0;
}


void LineFollow::operator()()
{
	auto line = _robot.ls.read();

	bool fJunc = delay_rising(_nJunc, line.state == LineSensors::Reading::JUNCTION, 3);
	bool fNone = delay_rising(_nNone, line.state == LineSensors::Reading::NONE, 5);
	bool fInvalid = delay_rising(_nInvalid, line.state == LineSensors::Reading::INVALID, 5);
	bool fLine = delay_rising(_nLine, line.state == LineSensors::Reading::LINE, 3);

	if (fInvalid)
		throw std::runtime_error("Sensor Error");


	switch (_state) {
		case BEGIN: // Initial State
			if (fJunc)
				_state = JUNCTION;
			else if (fNone)
				_state = LINE_LOST;
			else
				_state = ON_LINE;

			break;

		case ON_LINE: // On Line
			_robot.drive.move({
				forward: 0.8f,
				steer: 0.5f * line.position
			});

			if (fJunc)
				_state = JUNCTION;
			else if (fNone)
				_state = LINE_LOST;

			break;


		case LINE_LOST: // Off the line, attempt recovery
			_lineFinder.reset();
			_state = LINE_LOST1;
			break;

		case LINE_LOST1:
			// Todo: Timeout?
			_lineFinder();

			if (_lineFinder.found())
				_state = ON_LINE;
			else if (_lineFinder.completelyLost())
				_state = TOTALLY_LOST;

			break;

		case JUNCTION: // At junction (end state)
			// It's up to the caller to stop the motors.
			// This allows for smoother passing through junctions.
			break;

		case TOTALLY_LOST: // Completely Lost
			// Maybe try reversing?
			throw std::runtime_error("I got lost...");
			break;
	}
}


void LineFollow::gotoJunction(Robot& robot)
{
	LineFollow lf(robot);

	do {
		lf();
	} while (!lf.junction());
}


LineFinder::LineFinder(Robot& robot)
		: _robot(robot), _state(0), _nLine(0), _nInvalid(0), _timer(0)
{
}

void LineFinder::reset()
{
	_state = 0;
}

bool LineFinder::found() const
{
	return _state == LINE_FOUND;
}

bool LineFinder::completelyLost() const
{
	return _state == STILL_LOST;
}

void LineFinder::operator()()
{
	auto line = _robot.ls.read();

	bool fInvalid = delay_rising(_nInvalid, line.state == LineSensors::Reading::INVALID, 5);
	bool fLine = delay_rising(_nLine, line.state == LineSensors::Reading::LINE, 3);

	if (fInvalid)
		throw std::runtime_error("Sensor Error");

	switch (_state) {
		case BEGIN: // Initial State
			if (fLine)
				_state = LINE_FOUND;
			else {
				_robot.drive.move({
					forward: 0.0f,
					steer: -1.0f
				});

				_timer = timing::now();
				_state = SWEEP_LEFT;
			}

			break;

		case SWEEP_LEFT: // Sweep left 5s
			if (fLine)
				_state = LINE_FOUND;
			else if (timing::diff(_timer) > 5000.0) { // Goto sweep right
				_robot.drive.move({
					forward: 0.0f,
					steer: 1.0f
				});

				_timer = timing::now();
				_state = SWEEP_RIGHT;
			}
			break;
		case SWEEP_RIGHT: // Sweep right 10s
			if (fLine)
				_state = LINE_FOUND;
			else if (timing::diff(_timer) > 10000.0) {
				throw std::runtime_error("Line lost");
			}
			break;
	}
}


bool LineFinder::run(Robot& robot)
{
	// Todo: Timout check

	LineFinder lf(robot);

	do {
		lf();

		if (lf.found()) {
			return( true );
		}
	} while (!lf.completelyLost());

	return( false );
}