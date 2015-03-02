
#include <stdexcept>
#include "../util/signals.h"
#include "../timing.h"
#include "../robot.h"
#include "../util/signals.h"

#include "linefollow.h"


LineFollow::LineFollow(Robot& robot)
		: StateMachine(robot) _nJunc(0), _nNone(0), _nInvalid(0), _nLine(0), _lineFinder(robot)
{
}


bool LineFollow::junction() const
{
	return( _state == JUNCTION );
}

bool LineFollow::end() const
{
	return( _state == JUNCTION || _state = TOTALLY_LOST );
}


void LineFollow::_step()
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
			_lineFinder.step();

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

		default:
			throw std::logic_error("Impossible state reached.");
	}

	return;
}


LineFinder::LineFinder(Robot& robot)
		: StateMachine(robot), _nLine(0), _nInvalid(0)
{
}


bool LineFinder::found() const
{
	return _state == LINE_FOUND;
}


bool LineFinder::completelyLost() const
{
	return _state == STILL_LOST;
}


bool LineFinder::end() const
{
	return found() || completelyLost();
}


void LineFinder::_step()
{
	auto line = _robot.ls.read();

	bool fInvalid = delay_rising(_nInvalid, line.state == LineSensors::Reading::INVALID, 5);
	bool fLine = delay_rising(_nLine, line.state == LineSensors::Reading::LINE, 3);

	if (fInvalid)
		throw std::runtime_error("Sensor Error");

	switch (_state) {
		case BEGIN: // Initial State
			if (fLine) {
				_state = LINE_FOUND;
				break;
			}

			_state = (line.position < 0.0f) ? SWEEP_RIGHT1 : SWEEP_LEFT2;
			break;

		case SWEEP_LEFT1: // Sweep left 5s
		case SWEEP_RIGHT2: // Sweep right 5s
			if (_transition) {
				_robot.drive.move({
					forward: 0.0f,
					steer: (_state == SWEEP_LEFT1) ? -1.0f : 1.0f
				});
			}

			if (fLine) {
				_state = LINE_FOUND;
			}
			else if (_stateTime > 5000.0) {
				_state = (_state == SWEEP_LEFT1) ? SWEEP_RIGHT1 : SWEEP_LEFT2;
			}

			break;

		case SWEEP_RIGHT1: // Sweep right 10s
		case SWEEP_LEFT2: // Sweep left 10s
			if (_transition) {
				_robot.drive.move({
					forward: 0.0f,
					steer: (_state == SWEEP_RIGHT1) ? 1.0f : -1.0f
				});
			}

			if (fLine) {
				_state = LINE_FOUND;
			}
			else if (_stateTime > 10000.0) {
				_state = STILL_LOST;
			}

			break;

		case LINE_FOUND:
			break;

		case STILL_LOST:
			break;

		default:
			throw std::logic_error("Impossible state reached.");
	}

	return;
}
