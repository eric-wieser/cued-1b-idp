
#include "util/tracker.h"
#include "subroutines.h"

#include "util/logging.h"

void q6_return_from_d1(Robot& r) {
	Logger l("Q6: return to starting zone");
	turnAtJunction(r, 1, false);
	followUntil(r, 1, until_junction);
	turnAtJunction(r, 1);
	followUntil(r, 0.70, until_junction);
	followUntil(r, 0.25, until_junction);
	r.drive.straight(0.1).wait();
}
