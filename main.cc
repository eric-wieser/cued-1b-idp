
#include <iostream>

#include "robot.h"
#include "timing.h"
#include "rlink.h"
#include "logging.h"

void robotLogic(Robot& robot) {

}

int main() {
	RLink rlink;

	std::cout << "constructed" << std::endl;
	try {
		rlink.initialise();
	catch (LinkError& e) {
		ERR(std::cout << "link error: initialization failed" << endl);
		ERR(e.log());
		return -1;
	}

	try {
		if (rlink.request (TEST_INSTRUCTION) != TEST_INSTRUCTION_RESULT) {
			ERR(std::cout << "test instruction failed" << endl);
			return -1;
		}
	}
	catch (LinkError& e) {
		ERR(std::cout << "link error: test instruction" << endl);
		ERR(e.log());
		return -1;
	}

	INFO(std::cout << "initialised" << std::endl);

	Robot robot(rlink);

	for (;;) {
		timing::tick();
		try {
			robotLogic(robot);
		} catch (LinkError& e) {
			WARN(std::cout << "Unhandled link error" << std::endl);
			WARN(e.log());
			if(e.is_fatal) {
				WARN(std::cout << "Requires reinitialization" << std::endl);
				rlink.reinitialize();
				INFO(std::cout << "Reinitialized" << std::endl);
			}
		}
	}

	return(0);
}
