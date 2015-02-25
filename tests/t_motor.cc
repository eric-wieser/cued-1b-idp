#include "../dev/rlink.h"
#include "../dev/drive.h"
#include <robot_delay.h>
#include <cstdint>
#include <iostream>


int main() {
	RLink r;
	std::cout << "constructed" << std::endl;
	r.initialise();
	std::cout << "initialised" << std::endl;

	Drive d(r);

	std::cout << "Convert positive:" << Drive::convertSpeed(1.0f) << std::endl;
	std::cout << "Convert negative:" << Drive::convertSpeed(-1.0f) << std::endl;

	r.command(MOTOR_1_GO, Drive::convertSpeed(1.0f));
	r.command(MOTOR_2_GO, Drive::convertSpeed(-1.0f));
	delay(1000);
	r.command(MOTOR_1_GO, Drive::convertSpeed(-1.0f));
	r.command(MOTOR_2_GO, Drive::convertSpeed(1.0f));
	delay(1000);
	return 0;
}