#include "robot.h"
#include "subroutines.h"


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

	// turn
	r.drive.move({forward: 0.0f, steer: -1.0f});

	// wait till we leave the first line and arrive at the second
	waitForLine(r, negate { LineSensors::Reading::NONE });

	// wait till we leave the second line and arrive at the third
	waitForLine(r, negate { LineSensors::Reading::NONE });
}