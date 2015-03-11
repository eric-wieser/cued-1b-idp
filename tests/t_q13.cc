
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
			<< "Choose a start position:" << std::endl
			<< " 1 - start box" << std::endl
			<< " 2 - last egg" << std::endl
			<< " 3 - post-delivery" << std::endl;

		std::cout << "Your choice: ";
		int n = -1;
		std::cin >> n;

		if(n <= 1) {
			q1_collect_d2d3(r);
		}
		else {
			r.courier.recordEggAdded(EGG_WHITE);
			r.courier.recordEggAdded(EGG_TASTY);
			r.courier.recordEggAdded(EGG_WHITE);
		}
		if(n <= 2) q2_deliver_d2d3(r);
		q3_return_from_d2d3(r);
	}
}