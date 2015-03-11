
#include "util/tracker.h"
#include "subroutines.h"

void q5_deliver_d1(Robot& r) {
	// Rotate CCW 90 around left wheel

	r.drive.move({
		forward: 0.3,
		steer: 0.7
	});

	Timeout(r.drive.timeForTurn(90, 0.7)).wait();

	checkpoint(r, "Exploring the unknown");
	r.drive.straight(0.4).wait();

	// Turn CCW
	checkpoint(r, "Avoiding the ramp");
	r.drive.turn(20).wait();
	r.drive.straight(0.2);

	LineSensors::Reading reading;
	do {
		reading = r.ls.read();
	} while (!reading.lsc);

	checkpoint(r, "Going past the line");
	r.drive.straight(0.14).wait();

	checkpoint(r, "Spinning onto the line");
	r.drive.move({
		forward: 0.0f,
		steer: 1.0f
	});

	do {
		reading = r.ls.read();
	} while (!reading.lsc);

	checkpoint(r, "Going to a junction");
	goToJunction(r, 0.2, false);
}
