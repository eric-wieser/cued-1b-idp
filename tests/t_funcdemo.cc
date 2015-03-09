
#include "robot.h"
#include "routines/subroutines.h"
#include "util/tracker.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <robot_delay.h>


namespace demos {


void f1(Robot& r) {
	q2_collect_d2d3(r);
}

void f2(Robot& r) {
	Tracker::instance()->logPose(0.2, -0.8, 0);
	r.drive.move({forward: 1, steer: 0.5});

	delay(1000);
	while(r.ls.read().state == LineSensors::Reading::LINE);
	while(r.ls.read().state != LineSensors::Reading::LINE);
	std::cout << "Departed conveyor and found line" << std::endl;

	goToJunction(r, 1);
	turnAtJunction(r, true);
	std::cout << "Looking up ramp" << std::endl;

	goToJunction(r, 4);
	turnAtJunction(r, true);
	std::cout << "Looking along plateau" << std::endl;

	goToJunction(r, 1);
	r.drive.straight(0.1).wait();
	goToJunction(r, 1);
	std::cout << "At delivery nexus" << std::endl;
}

void f3(Robot& r) {
	std::cout << "At delivery nexus" << std::endl;
	Tracker::instance()->logPose(-1.2 + 0.36 + 0.2, 1.2 - 0.36, M_PI);
	// add some virtual eggs if we don't yet have any
	if(r.courier.volume() < 3) r.courier.recordEggAdded(EGG_BROWN);
	if(r.courier.volume() < 3) r.courier.recordEggAdded(EGG_TASTY);

	// rotate to D2 delivery
	r.drive.straight(0.1).wait();

	dropWhite(r);
	dropCreme(r);
	dropWhite(r);
}

void all(Robot& r) {
	f1(r);
	f2(r);
	f3(r);
}


} // namespace demos


int main() {
	RLink link;
	std::cout << "constructed" << std::endl;
	link.initialise();
	std::cout << "initialised" << std::endl;

	while(1) {
		Robot r(link);

		std::cout
			<< "Choose a demo:" << std::endl
			<< " 1 - drive along conveyor" << std::endl
			<< " 2 - follow line up ramp" << std::endl
			<< " 3 - deliver eggs" << std::endl
			<< " 0 - all of the above" << std::endl;

		std::cout << "Your choice: ";
		int n = -1;
		std::cin >> n;

		if(n == 1) demos::f1(r);
		if(n == 2) demos::f2(r);
		if(n == 3) demos::f3(r);
		if(n == 0) demos::all(r);

		if(n == -1) break;
	}
}