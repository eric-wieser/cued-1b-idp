#include "rlink.h"
#include "robot_instr.h"
#include "robot_delay.h"
#include <iostream>

int main() {
	RLink r;
	r.initialise();

	std::cout << "initialised" << std::endl;

	r.command(MOTOR_1_GO, 127);
	std::cout << "going" << std::endl;
	delay(1000000);
	std::cout << "done" << std::endl;
}