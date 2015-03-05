#include "dev/ports.h"
#include "dev/rlink.h"
#include <cstdint>
#include <iostream>

#include "robot_delay.h"

const int lift = (1 << 6);
const int grab = (1 << 7);

int main() {
	RLink r;
	std::cout << "constructed" << std::endl;
	r.initialise();
	std::cout << "initialised" << std::endl;

	Port actuator(r, port::P2, lift | grab);
	actuator = 0x00;

	std::cout << "constructed ports" << std::endl;

	int b = 0;
	while(true) {
		actuator = 0x00;
		std::cout << "off" << std::endl;
		delay(1000);
		actuator = lift;
		std::cout << "on1" << std::endl;
		delay(1000);
		actuator = lift | grab;
		std::cout << "on2" << std::endl;
		delay(1000);
		actuator = grab;
		std::cout << "on3" << std::endl;
		delay(1000);
	}
}