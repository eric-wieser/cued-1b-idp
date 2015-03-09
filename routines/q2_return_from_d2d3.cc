
#include <stdexcept>

#include "dev/drive.h"
#include "util/tracker.h"
#include "common.h"

void q2_return_from_d2d3(Robot& r)
{
	turnAtJunction(r, 90);
	turnAtJunction(r, 90);
	goToJunction(r, 0.86);
	goToJunction(r, 0.86);
	std::cout << "At plateau start" << std::endl;

	turnAtJunction(r, -90);
	goToJunction(r, 99999999); // ramps takes forever, so just drop the timeout

	turnAtJunction(r, -90);
	goToFullJunction(r, 0.84);
	goToJunction(r, 0.84 - 0.20);
	goToJunction(r, 0.84 - 0.20);
}
