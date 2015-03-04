#include "../robot.h"
#include "../subroutines.h"
#include "../logic/linefollow.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <robot_delay.h>
namespace sm {
	void goToJunction(Robot& r, float d) {
		LineFollow lf;

		lf.run();

		if (!lf.junction()) {
			throw std::runtime_error("I got lost...");
		}
	}
}

int main() {
	RLink link;
	std::cout << "constructed" << std::endl;
	link.initialise();
	std::cout << "initialised" << std::endl;

	Robot r(link);

	Drive::move_args args = {forward: 1, steer: -0.2};

	goToConveyor(r);

	for(int i  = 0; i < 5; i++) {
		r.drive.move(args);
		while(r.ls.read().lsl == 1);
		while(r.ls.read().lsl == 0);
		r.drive.stop();

		std::cout << "At station " << i << std::endl;
		delay(500);
	}
	r.drive.move({forward: 1, steer: 0.5});

	delay(1000);
	while(r.ls.read().state == LineSensors::Reading::LINE);
	while(r.ls.read().state != LineSensors::Reading::LINE);
	std::cout << "Departed conveyor and found line" << std::endl;

	sm::goToJunction(r, 1);
	turnAtJunction(r);
	std::cout << "Looking up ramp" << std::endl;

	sm::goToJunction(r, 4);
	turnAtJunction(r);
	std::cout << "Looking along plateau" << std::endl;

	sm::goToJunction(r, 1);
	r.drive.straight(0.1).wait();
	sm::goToJunction(r, 1);
	std::cout << "At delivery nexus" << std::endl;

	r.drive.straight(0.1).wait();
	r.drive.move({forward: 0.8, steer: 1});

	while(r.ls.read().state == LineSensors::Reading::LINE);
	while(r.ls.read().state != LineSensors::Reading::LINE);
	std::cout << "At D2" << std::endl;
	r.drive.stop();
}