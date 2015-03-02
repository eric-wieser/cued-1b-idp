
#include "../dev/drive.h"


#include "q2_collect_d2d3.h"



q2_collect_d2d3::q2_collect_d2d3(Robot& robot)
		: StateMachine(robot)
{
}


void q2_collect_d2d3::operator()()
{
	auto ls = _robot.ls.read();

	switch(_state) {
		case 0: // Initial State
			state(10);
			break;

		case 10: // Rotating about left wheel
			if (_transition) {
				_robot.drive.setWheelSpeeds(0.0f, 1.0f);
			}

			if (ls.lsc) {
				state(11);
			}
			break;

		case 11: // Reverse to 1st junction
			if (_transition) {
				_robot.drive.move({
					forward: -1.0f,
					steer: 0.0f
				});
			}

			if (ls.state == LineSensors::Reading::JUNCTION) {
				state(12);
			}
			break;

		case 12: // Reverse through junction
			if (ls.state != LineSensors::Reading::JUNCTION) {
				state(13);
			}
			break;

		case 13: // Reverse to 2nd junction
			if (ls.state == LineSensors::Reading::JUNCTION) {
				state(14);
			}
			break;
		case 14: // Rotate on the spot
			if (_transition) {
				_robot.drive.setWheelSpeeds(0.5f, -0.5f);
			}

			if (ls.lsc) {
				state(15);
			}

			break;
		case 15: // Forwards
			if (_transition) {
				_robot.drive.move({
					forward: 1.0f,
					steer: 0.0f
				});
			}

			if (ls.lsa) {
				state(20);
			}
			break;

		case 20: // Lower Arm
			state(-1);
			break;

		case 30: // Check Egg
			break;

		case 40: // Grab Egg
			break;

		case 50: // Raise Arm
			break;

		case 60: // Move Forwards
			if (_transition) {
				_robot.drive.move({
					forward: 1.0f,
					steer: 0.0f
				});
			}

			if (ls.lsa) {
				state(20);
			}

			break;

	}

	_transition = false;
}