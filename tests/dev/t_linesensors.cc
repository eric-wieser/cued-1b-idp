#include "dev/rlink.h"
#include "dev/linesensors.h"
#include <cstdint>
#include <iostream>
#include <string>

int main() {
	RLink r;
	std::cout << "constructed" << std::endl;
	r.initialise();
	std::cout << "initialised" << std::endl;

	LineSensors ls(r, port::P1);

	while(1) {
		auto reading = ls.read();
		std::cout << "line position: " << reading.position << "       \r";
		std::cout.flush();
	}
	return 0;
}