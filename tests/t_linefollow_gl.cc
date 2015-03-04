
#include <iostream>
#include <stdexcept>
#include <chrono>

#include "../robot.h"
#include "../logging.h"
#include "../util/timing.h"
#include "../dev/rlink.h"
#include "../util/signals.h"


struct robotLogic
{
	int state;
	int nJunc;
	int nNone;
	int nInvalid;
	int nLine;
	t_timer timer;

	void operator()(CachingRobot& robot) {
		auto line = robot.ls();

		bool fJunc = delay_rising(nJunc, line.state == LineSensors::Reading::JUNCTION, 3);
		bool fNone = delay_rising(nNone, line.state == LineSensors::Reading::NONE, 5);
		bool fInvalid = delay_rising(nInvalid, line.state == LineSensors::Reading::INVALID, 5);
		bool fLine = delay_rising(nLine, line.state == LineSensors::Reading::LINE, 3);

		switch (state) {
			case 0: // Initial State
				if (fJunc)
					state = 30;
				else if (fNone)
					state = 20;
				else if (fInvalid)
					throw std::runtime_error("Sensor Error");
				else
					state = 1;

				break;
			case 1: // On Line
				robot.drive().move({
					forward: 0.8f,
					steer: 0.5f * line.position
				});

				if (fJunc)
					state = 30;
				else if (fInvalid)
					throw std::runtime_error("Sensor Error");
				else if (fNone)
					state = 20;

				break;


			case 20: // Off the line, attempt recovery
				// Thinking probably swing left 5s, swing right 10s, and stop if you don't find it either way
				robot.drive().move({
					forward: 0.1f,
					steer: -1.0f
				});

				timer = timing::now();
				state = 21;

				break;
			case 21: // Swinging left
				if (fLine) 
					state = 1;
				else if (timing::diff(timer) > 5000.0) {
					robot.drive().move({
						forward: 0.1f,
						steer: 1.0f
					});

					timer = timing::now();
					state = 22;
				}
				break;
			case 22: // Swinging Right
				if (fLine)
					state = 1;
				else if (timing::diff(timer) > 10000.0) {
					throw std::runtime_error("Line lost");
				}
				break;


			case 30: // At junction (end state)
				break;
		}
	}
};

int main() {
	RLink rlink;

	std::cout << "constructed" << std::endl;
	try {
		rlink.initialise();
	}
	catch (LinkError& e) {
		ERROR(std::cout << "link error: initialization failed" << std::endl);
		ERROR(e.log());
		return -1;
	}

	try {
		if (rlink.request (TEST_INSTRUCTION) != TEST_INSTRUCTION_RESULT) {
			ERROR(std::cout << "test instruction failed" << std::endl);
			return -1;
		}
	}
	catch (LinkError& e) {
		ERROR(std::cout << "link error: test instruction" << std::endl);
		ERROR(e.log());
		return -1;
	}

	INFO(std::cout << "initialised" << std::endl);

	CachingRobot robot(rlink);

	for (;;) {
		timing::tick();
		try {
			robotLogic(robot);
		} catch (LinkError& e) {
			WARN(std::cout << "Unhandled link error" << std::endl);
			WARN(e.log());
			if(e.is_fatal) {
				WARN(std::cout << "Requires reinitialization" << std::endl);
				// rlink.reinitialize();
				INFO(std::cout << "Reinitialized" << std::endl);
			}
		}
	}

	return(0);
}
