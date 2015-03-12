#include "util/tracker.h"
#include "subroutines.h"

void q3_return_from_d2d3(Robot& r) {
	turnAtJunction(r, -2, false);
	followUntil(r, 0.84, until_junction);
	checkpoint(r, "At plateau center");

	followUntil(r, 0.84, until_junction);
	checkpoint(r, "At plateau start");

	turnAtJunction(r, -1);
	followUntil(r, 99999999, until_junction); // ramps takes forever, so just drop the timeout
	checkpoint(r, "At ramp bottom");

	turnAtJunction(r, -1);
	followUntil(r, 0.84, until_xjunction);
	checkpoint(r, "At conveyor cewnter");

	followUntil(r, 0.84 - 0.20, until_xjunction);
	checkpoint(r, "At start box edge");

	followUntil(r, 0.20, until_junction);
	checkpoint(r, "At start box center");

	r.drive.straight(0.05).wait();
	r.drive.stop();
}
