
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

	while(1) {
		Robot r(link);

		std::cout
			<< "Choose a demo:" << std::endl
			<< " 1 - it" << std::endl;

		std::cout << "Your choice: ";
		int n = -1;
		std::cin >> n;

		q1_collect_d2d3(r);
		q2_deliver_d2d3(r);
		q3_return_from_d2d3(r);
	}
}