
#include "util/tracker.h"
#include "subroutines.h"

void q6_return_from_d1(Robot& r) {
	turnAtJunction(r, 1, false);
	goToJunction(r, 1);
	turnAtJunction(r, 1);
	goToJunction(r, 0.70);
	goToJunction(r, 0.25);
	r.drive.straight(0.1).wait();
}
