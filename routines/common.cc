#include "robot.h"
#include "util/tracker.h"

#include "common.h"


void waitForLine(Robot& r, LineSensors::Reading::State s) {
	while(r.ls.read().state == s);
	while(r.ls.read().state != s);
}

void waitForLine(Robot& r, negate n) {
	while(r.ls.read().state != n.s);
	while(r.ls.read().state == n.s);
}

void dropCreme(Robot& r) {
	r.drive.move({forward: 0.5, steer: 0});
	while(!(r.limits & (1 << 5)));
	r.drive.stop();

	std::cout << "At D3" << std::endl;
	r.courier.unloadEgg();

	r.drive.straight(-0.1, 0.5).wait();

}

void dropWhite(Robot& r) {
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
