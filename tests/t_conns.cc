#include "dev/ports.h"
#include "dev/rlink.h"
#include <cstdint>
#include <iostream>

#include "robot_delay.h"

int main() {
	RLink r;
	std::cout << "constructed" << std::endl;
	r.initialise();
	std::cout << "initialised" << std::endl;

	for(int pn = port::P1; pn <= port::P3; pn++) {
		try {
			std::cout << "Testing " << pn << "... ";
			std::cout.flush();
			Port p(r, (port::Name) pn);
			uint8_t x = p;
			std::cout << "Connected" << std::endl;
		}
		catch(PortError &e) {
			std::cout << "Disconnected" << std::endl;
		}
	}
	std::cout << "Done" << std::endl;
}