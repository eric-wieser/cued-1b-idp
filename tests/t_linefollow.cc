#include "../robot.h"
#include "../subroutines.h"

#include <cstdint>
#include <iostream>
#include <string>

int main() {
	RLink link;
	std::cout << "constructed" << std::endl;
	link.initialise();
	std::cout << "initialised" << std::endl;

	Robot r(link);

	try {
		goToJunction(r, 20);
	}
	catch(std::exception& e) {
		std::cout << "Damn: " << e.what() << std::endl;
	}
	return 0;
}