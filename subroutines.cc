#include "robot.h"
#include "subroutines.h"
#include "util/tracker.h"


void waitForLine(Robot& r, LineSensors::Reading::State s) {
	while(r.ls.read().state == s);
	while(r.ls.read().state != s);
}

void waitForLine(Robot& r, negate n) {
	while(r.ls.read().state != n.s);
	while(r.ls.read().state == n.s);
}

void goToConveyor(Robot& r) {
	r.drive.move({forward: 1.0f, steer: 1.0f});

	// wait till we leave the first line and arrive at the second
	waitForLine(r, negate { LineSensors::Reading::NONE });

	// wait till we leave the second line and arrive at the third
	waitForLine(r, negate { LineSensors::Reading::NONE });

	r.drive.move({forward: -1.0f, steer: 0.0f});

	// wait for the first junction
	waitForLine(r, LineSensors::Reading::JUNCTION);

	// wait for the second junction
	waitForLine(r, LineSensors::Reading::JUNCTION);

	// wait for the third junction
	waitForLine(r, LineSensors::Reading::JUNCTION);


	// turn
	r.drive.move({forward: 1.0f, steer: -1.0f});
	Timeout(std::chrono::duration<float>(180 / r.drive.maxSpeeds.rotary)).wait();
	Timeout(std::chrono::duration<float>(1)).wait();
}

void demos::f1(Robot& r) {
	Drive::move_args args = {forward: 1, steer: -0.2};

	r.arm.open();
	r.arm.up();

	goToConveyor(r);
	r.drive.stop();

	for(int i  = 0; i < 5; i++) {
		r.drive.move(args);
		while(r.ls.read().lsl == 1);
		while(r.ls.read().lsl == 0);
		r.drive.stop();
		Tracker::instance()->logPose(-0.4 + 0.2 * i - 0.2, -0.8, 0);

		delay(100);
		auto reading = r.detector.read();
		std::cout << "Egg at station " << i << ": " << reading.bestGuess << std::endl;

		if (reading.bestGuess == EGG_WHITE) {
			r.drive.straight(0.1).wait();
			r.drive.stop();
			
			r.arm.down();
			r.arm.close();
			r.arm.up();
			r.arm.open();
		}
	}

}

void demos::f2(Robot& r) {
	Tracker::instance()->logPose(0.2, -0.8, 0);
	r.drive.move({forward: 1, steer: 0.5});

	delay(1000);
	while(r.ls.read().state == LineSensors::Reading::LINE);
	while(r.ls.read().state != LineSensors::Reading::LINE);
	std::cout << "Departed conveyor and found line" << std::endl;

	goToJunction(r, 1);
	turnAtJunction(r);
	std::cout << "Looking up ramp" << std::endl;

	goToJunction(r, 4);
	turnAtJunction(r);
	std::cout << "Looking along plateau" << std::endl;

	goToJunction(r, 1);
	r.drive.straight(0.1).wait();
	goToJunction(r, 1);
	std::cout << "At delivery nexus" << std::endl;
}

static void dropCreme(Robot& r) {
	r.drive.move({forward: 0.5, steer: 0});
	while(!(r.limits & (1 << 5)));
	r.drive.stop();

	std::cout << "At D3" << std::endl;
	r.courier.unloadEgg();

	r.drive.straight(-0.1, 0.5).wait();

}

static void dropWhite(Robot& r) {
	r.drive.move({forward: 0.7, steer: 1});

	while(r.ls.read().lsc);
	while(!r.ls.read().lsc);
	r.drive.stop();

	r.drive.move({forward: 0.5, steer: 0});
	while(!(r.limits & (1 << 5)));
	r.drive.stop();

	std::cout << "At D2" << std::endl;
	r.courier.unloadEgg();

	r.drive.straight(-0.05, 0.5).wait();

	// rotate back to nexus, then drive to D3
	r.drive.move({forward: -0.7, steer: -1});
	while(r.ls.read().state == LineSensors::Reading::LINE);
	while(r.ls.read().state != LineSensors::Reading::LINE);
	r.drive.stop();
}

void demos::f3(Robot& r) {
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

void demos::all(Robot& r) {
	f1(r);
	f2(r);
	f3(r);
}
