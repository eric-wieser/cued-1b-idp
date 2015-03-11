#include "util/tracker.h"
#include "subroutines.h"


void q2_deliver_d2d3(Robot& r) {
	Tracker::instance()->logPose(0.2, -0.8, 0);
	r.drive.move({forward: 1, steer: 0.5});

	delay(1000);
	waitForLine(r, negate { LineSensors::Reading::NONE });
	std::cout << "Departed conveyor and found line" << std::endl;

	goToJunction(r, 0.30);
	checkpoint(r, "Junction Before Ramp");
	turnAtJunction(r, 1);
	checkpoint(r, "Looking up ramp");

	goToJunction(r, 999999); // ramps takes forever, so just drop the timeout
	checkpoint(r, "Top of ramp");

	turnAtJunction(r, 1);
	checkpoint(r, "Looking along plateau");

	goToJunction(r, 0.86);
	checkpoint(r, "At plateau center");

	goToJunction(r, 0.86);
	r.drive.straight(0.1).wait();
	checkpoint(r, "At delivery nexus");

	while(r.courier.volume() != 0) {
		if(r.courier.egg(0) == EGG_WHITE) {
			// curve left, until we intersect the line
			r.drive.move({forward: 0.7, steer: 1});
			while(r.ls.read().lsc);
			while(!r.ls.read().lsc);
			r.drive.stop();

			// run until the limit switches go
			dropEggs(r, r.courier.egg(1) == EGG_WHITE ? 2 : 1);

			// undo the curved motion
			r.drive.move({forward: -0.7, steer: -1});
			while(r.ls.read().lsc);
			while(!r.ls.read().lsc);
			r.drive.stop();
		}
		else {
			dropEggs(r, 1);
		}
	}

	r.drive.straight(-0.1).wait();
	std::cout << "At delivery nexus" << std::endl;
}
