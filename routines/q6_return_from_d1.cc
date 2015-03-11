
#include "util/tracker.h"
#include "subroutines.h"

void q6_return_from_d1(Robot& r) {
	r.drive.straight(-0.2).wait();
	turnAtJunction(r, 1);
	goToJunction(r, 0.84);
	turnAtJunction(r, 1);
	goToJunction(r, 0.70);
	goToJunction(r, 0.15);
	r.drive.straight(0.1).wait();
}
