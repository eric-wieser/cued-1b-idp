#include "rlink.h"
#include "robot_instr.h"
#include "robot_delay.h"
#include <iostream>

#include "drive.h"

int main() {
	RLink r;
	std::cout << "constructed" << std::endl;
	r.initialise();
	std::cout << "initialised" << std::endl;

	Drive drive(&r);

	float f = 1;
	while(true) {
		std::cout << "ready..." << std::endl;
		std::cin >> f;
		std::cout << "going" << std::endl;
		drive.move({forward: f, steer: 0});
	}

	std::cout << "done" << std::endl;
}