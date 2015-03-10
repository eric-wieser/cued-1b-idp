
#include "util/tracker.h"
#include "subroutines.h"

void q6_return_from_d1(Robot& r) {
	turnAtJunction(r, 1);
	goToJunction(r, 0.30);
	turnAtJunction(r, 1);
	goToJunction(r, 0.50);
	goToJunction(r, 0.10);
	turnAtJunction(r, 1);
	// Reverse?
}
