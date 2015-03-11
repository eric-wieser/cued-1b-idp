
#include "robot.h"
#include "routines/subroutines.h"
#include "util/tracker.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <robot_delay.h>



int main() {
	RLink link;
	std::cout << "constructed" << std::endl;
	link.initialise();
	std::cout << "initialised" << std::endl;

	Robot r(link);

	link.command(MOTOR_3_GO, Drive::convertSpeed(-0.5));

	r.courier.recordEggAdded(EGG_TASTY);
	r.courier.recordEggAdded(EGG_WHITE);
	r.courier.recordEggAdded(EGG_BROWN);

	dropEggs(r, 2);
}
