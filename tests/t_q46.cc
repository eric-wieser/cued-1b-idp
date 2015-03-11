
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
			q4_collect_d1(r);
		}
		else {
			r.courier.recordEggAdded(EGG_WHITE);
			r.courier.recordEggAdded(EGG_WHITE);
		}

		if(n <= 2) q5_deliver_d1(r);
		if(n <= 3) q6_return_from_d1(r);
	}
}
