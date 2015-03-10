
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

	goToJunction(r, 1);
	std::cout << "Junction" << std::endl;
	turnAtJunction(r, true);
	std::cout << "Turned" << std::endl;
	goToJunction(r, 1);
	std::cout << "Junction" << std::endl;
}
