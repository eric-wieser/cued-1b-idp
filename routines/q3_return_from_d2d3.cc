#include "util/tracker.h"
#include "subroutines.h"

void q3_return_from_d2d3(Robot& r) {
	turnAtJunction(r, 2, false);
	goToJunction(r, 0.86);
	goToJunction(r, 0.86);
	std::cout << "At plateau start" << std::endl;

	turnAtJunction(r, -1);
	goToJunction(r, 99999999); // ramps takes forever, so just drop the timeout

	turnAtJunction(r, -1);
	goToJunction(r, 0.84, until_xjunction);
	goToJunction(r, 0.84 - 0.20, until_xjunction);
	goToJunction(r, 0.20);
	r.drive.straight(0.05).wait();
	r.drive.stop();
}
