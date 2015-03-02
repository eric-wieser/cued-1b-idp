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

	Port actuator(r, port::P3, (1 << 6) | (1 << 7));
	actuator = 0x00;

	std::cout << "constructed ports" << std::endl;

	int b = 0;
	while(true) {
		actuator = 0x00;
		delay(1000);
		actuator = 0xff;
		delay(1000);
	}
}