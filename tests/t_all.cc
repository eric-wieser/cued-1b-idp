
#include "robot.h"
#include "routines/subroutines.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <robot_delay.h>

#include <typeinfo>



int main() {
	RLink link;
	std::cout << "constructed" << std::endl;
	link.initialise();
	std::cout << "initialised" << std::endl;

	while(1) {
		try {
			Robot r(link);

			std::cout
				<< "Choose a start position:" << std::endl
				<< " Q1 - collect D2 and D3     - start box, facing in" << std::endl
				<< " Q2 - deliver D2 and D3     - last egg location" << std::endl
				<< " Q3 - return from D2 and D3 - delivery crossroads, facing D3" << std::endl
				<< " Q4 - collect remainder     - start box, facing out" << std::endl
				<< " Q5 - deliver remainder     - last egg location" << std::endl
				<< " Q3 - return from D1        - facing D1" << std::endl;

			std::cout << "Your choice: ";
			int n = -1;
			std::cin >> n;

			if(n <= 1) q1_collect_d2d3(r);
			if(n == 2) {
				r.courier.recordEggAdded(EGG_BROWN);
				r.courier.recordEggAdded(EGG_TASTY);
				r.courier.recordEggAdded(EGG_BROWN);
			}
			if(n <= 2) q2_deliver_d2d3(r);
			if(n <= 3) q3_return_from_d2d3(r);

			if(n <= 4) q4_collect_d1(r);
			if(n == 5) {
				r.courier.recordEggAdded(EGG_WHITE);
				r.courier.recordEggAdded(EGG_WHITE);
			}
			if(n <= 5) q5_deliver_d1(r);
			if(n <= 6) q6_return_from_d1(r);
		}
		catch(std::exception &e) {
			std::cout << "Caught " << typeid(e).name() << ":" << e.what() << std::endl;
			std::cout << "Restarting" << std::endl;
		}
	}
}