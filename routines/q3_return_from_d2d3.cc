#include "util/tracker.h"
#include "subroutines.h"

void q3_return_from_d2d3(Robot& r) {
	turnAtJunction(r, -2, false);
	goToJunction(r, 0.84);
	checkpoint(r, "At plateau center");
	goToJunction(r, 0.84);
	checkpoint(r, "At plateau start");

	turnAtJunction(r, -1);
	goToJunction(r, 99999999); // ramps takes forever, so just drop the timeout
	checkpoint(r, "At ramp bottom");

	turnAtJunction(r, -1);
	goToJunction(r, 0.84, until_xjunction);
	checkpoint(r, "At conveyor cewnter");

	goToJunction(r, 0.84 - 0.20, until_xjunction);
	checkpoint(r, "At start box edge");

	goToJunction(r, 0.20);
	checkpoint(r, "At start box center");

	r.drive.straight(0.05).wait();
	r.drive.stop();
}
