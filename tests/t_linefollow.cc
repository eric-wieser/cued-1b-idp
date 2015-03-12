
#include "robot.h"
#include "routines/subroutines.h"

#include <cstdint>
#include <iostream>
#include <string>

int main() {
	RLink link;
	std::cout << "constructed" << std::endl;
	link.initialise();
	std::cout << "initialised" << std::endl;

	Robot r(link);

	followUntil(r, 1, until_junction);
	std::cout << "Junction" << std::endl;
	turnAtJunction(r, true);
	std::cout << "Turned" << std::endl;
	followUntil(r, 1, until_junction);
	std::cout << "Junction" << std::endl;
}
