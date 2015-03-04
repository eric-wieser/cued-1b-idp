
#include <stdexcept>
#include "../dev/drive.h"


#include "q2_collect_d2d3.h"



q2_collect_d2d3::q2_collect_d2d3(Robot& robot)
		: StateMachine(robot), currentEgg(EGG_NONE), conveyorPos(0)
{
}


void q2_collect_d2d3::_step()
{
	auto ls = _robot.ls.read();

	switch(_state) {
		case 0: // Initial State
			_state = 10;
			break;

		case 10: // Rotating about left wheel
			if (_transition) {
				_robot.drive.setWheelSpeeds(0.0f, 1.0f);
			}

			if (!ls.lsc) {
				_state = 11;
			}
			break;

		case 11: // Keep Rotating
			if (ls.lsc) {
				_state = 12;
			}
			break;

		case 12: // Keep Rotating
			if (!ls.lsc) {
				_state = 13;
			}
			break;

		case 13: // Keep Rotating
			if (ls.lsc) {
				_state = 14;
			}
			break;


		case 14: // Reverse to 1st junction
			if (_transition) {
				_robot.drive.move({
					forward: -1.0f,
					steer: 0.0f
				});
			}

			if (ls.state == LineSensors::Reading::JUNCTION) {
				_state = 15;
			}
			break;

		case 15: // Reverse through junction
			if (ls.state != LineSensors::Reading::JUNCTION) {
				_state = 16;
			}
			break;

		case 16: // Reverse to 2nd junction
			if (ls.state == LineSensors::Reading::JUNCTION) {
				_state = 17;
			}
			break;
		case 17: // Rotate on the spot
			if (_transition) {
				_robot.drive.setWheelSpeeds(0.5f, -0.5f);
			}

			if (ls.lsc) {
				_state = 20;
			}

			break;
		case 20: // Forwards
			if (_transition) {
				_robot.drive.move({
					forward: 1.0f,
					steer: 0.0f
				});
			}

			if (ls.lsa) {
				_robot.drive.stop();
				conveyorPos++;
				_state = 30;
			}
			break;

		case 30: // Check Egg
			currentEgg = _robot.detector.read().bestGuess;
			// Retry if failed?

			switch (currentEgg) {
				case EGG_BROWN:
				case EGG_TASTY:
					_state = 40;
					break;
				default:
					_state = 20;
					break;
			}
			break;

		case 40: // Forwards to Arm sensor
			if (_transition) {
				_robot.drive.move({
					forward: 1.0f,
					steer: 0.0f
				});
			}

			if (ls.lsa) {
				_robot.drive.stop();
				_state = 50;
			}
			break;

		case 50: // Lower Arm
			// Power Actuator 1 Bore

			_state = 60;
			break;

		case 60: // Grab Egg
			// Power Actuator 2 Bore

			_state = 70;
			break;

		case 70: // Raise Arm
			// Power Actuator 1 Annulus

			_robot.eggTypes[_robot.eggsLoaded] = currentEgg;
			_robot.eggsLoaded++;

			if (_robot.eggsLoaded == 3
					|| conveyorPos == 5) {
				_state = -1;
			}
			else {
				_state = 20;
			}
			break;

		default:
			throw std::logic_error("Invalid state reached.");
	}

	return;
}