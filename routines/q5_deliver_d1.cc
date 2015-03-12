
#include "util/tracker.h"
#include "subroutines.h"

#include "util/logging.h"

void q5_deliver_d1(Robot& r) {
	Logger l("Q5: deliver D1 + rest");

	// Rotate CCW 90 around left wheel

	r.drive.move({
		forward: 0.5,
		steer: 0.5
	});

	Timeout(r.drive.timeForTurn(90, 0.5)).wait();

	l.checkpoint(r, "Exploring the unknown");
	r.drive.straight(0.30).wait();

	// Turn CCW
	l.checkpoint(r, "Avoiding the ramp");
	r.drive.turn(40).wait();
	r.drive.straight(0.1).wait();

	LineSensors::Reading reading;
	do {
		reading = r.ls.read();
	} while (!reading.lsc);

	l.checkpoint(r, "Going past the line");
	r.drive.straight(0.14).wait();

	l.checkpoint(r, "Spinning onto the line");
	r.drive.move({
		forward: 0.0f,
		steer: 1.0f
	});

	do {
		reading = r.ls.read();
	} while (!reading.lsc);

	l.checkpoint(r, "Going to a junction");
	followUntil(r, 0.2, until_junction);

	l.checkpoint(r, "Turning Right");
	turnAtJunction(r, -1);

	l.checkpoint(r, "Going to the box");
	dropEggs(r, r.courier.volume());
}
