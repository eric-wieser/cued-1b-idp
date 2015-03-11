
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

	q4_collect_d1(r);
}
