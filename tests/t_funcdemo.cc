#include "robot.h"
#include "subroutines.h"
// #include "../logic/linefollow.h"

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
			<< " 1 - drive along conveyor" << std::endl
			<< " 2 - follow line up ramp" << std::endl
			<< " 3 - deliver eggs" << std::endl
			<< " 0 - all of the above" << std::endl;

		std::cout << "Your choice: ";
		int n = -1;
		std::cin >> n;

		if(n == 1) demos::f1(r);
		if(n == 2) demos::f2(r);
		if(n == 3) demos::f3(r);
		if(n == 0) demos::all(r);

		if(n == -1) break;
	}
}