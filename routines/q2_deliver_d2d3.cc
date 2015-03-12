#include "util/tracker.h"
#include "subroutines.h"

#include "util/logging.h"


void q2_deliver_d2d3(Robot& r) {
	Logger l("Q2: deliver D2 + D3");

	Tracker::instance()->logPose(0.2, -0.8, 0);
	r.drive.move({forward: 1, steer: 0.5});

	delay(1000);
	waitForLine(r, negate { LineSensors::Reading::NONE });

	l.checkpoint(r, "Departed conveyor and found line");

	followUntil(r, 0.30, until_junction);
	l.checkpoint(r, "Junction Before Ramp");

	turnAtJunction(r, 1);
	l.checkpoint(r, "Looking up ramp");

	followUntil(r, 999999, until_junction); // ramps takes forever, so just drop the timeout
	l.checkpoint(r, "Top of ramp");

	turnAtJunction(r, 1);
	l.checkpoint(r, "Looking along plateau");

	followUntil(r, 0.86, until_junction);
	l.checkpoint(r, "At plateau center");

	followUntil(r, 0.86, until_junction);
	r.drive.straight(0.1).wait();
	l.checkpoint(r, "At delivery nexus");

	while(r.courier.volume() != 0) {
		if(r.courier.egg(0) == EGG_BROWN) {
			// curve left, until we intersect the line
			r.drive.move({forward: 0.35, steer: 0.65});
			Timeout(r.drive.timeForTurn(45, 0.65)).wait();

			while(r.ls.read().lsc);
			while(!r.ls.read().lsc);
			r.drive.stop();

			// run until the limit switches go
			int n = 1;
			if(r.courier.egg(n) == EGG_BROWN) n++;
			if(r.courier.egg(n) == EGG_BROWN) n++;
			dropEggs(r, n);

			// undo the curved motion
			r.drive.move({forward: -0.35, steer: -0.65});
			Timeout(r.drive.timeForTurn(-45, 0.65)).wait();
			while(r.ls.read().lsc);
			while(!r.ls.read().lsc);
			r.drive.stop();
		}
		else {
			dropEggs(r, 1);
		}
	}
	l.checkpoint(r, "Returned to delivery nexus");

	r.drive.straight(-0.3).wait();
}
