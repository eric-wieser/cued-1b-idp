
#include <iostream>
#include <stdexcept>

#include "../robot.h"
#include "../logging.h"
#include "../timing.h"
#include "../dev/rlink.h"
#include "../logic/linefollow.h"

struct RobotLogic
{
	Robot& robot;
	LineFollow lf;

	RobotLogic(Robot& r) : robot(r), lf(r) {}

	void operator()() {
		lf();

		if (lf.junction()) {
			robot.drive.stop();
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

	Robot robot(rlink);
	RobotLogic robotLogic(robot);

	for (;;) {
		try {
			robotLogic();
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
