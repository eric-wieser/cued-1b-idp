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

	Port leds(r, port::P0, 0x07);
	Port button(r, port::P0, 0x08);
	button = 0x08;

	std::cout << "constructed ports" << std::endl;

	int i = 0;
	while(true) {
		int last = -1;
		while(!button);
		delay(250);
		while(button);
		i++;

		leds = ~(1 << (i % 4));
		button = 0x08;
	}
}