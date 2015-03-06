
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
	turnAtJunction(r);
	goToJunction(r, 1);
}
