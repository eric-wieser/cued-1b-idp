
#include <iostream>

#include "robot.h"
#include "timing.h"
#include "rlink.h"

void robotLogic(Robot& robot) {

}

int main() {
	RLink rlink;

	std::cout << "constructed" << std::endl;
	rlink.initialise();
	std::cout << "initialised" << std::endl;

	Robot robot(rlink);

	for (;;) {
		timing::tick();

		robotLogic(robot);
	}

	return(0);
}
