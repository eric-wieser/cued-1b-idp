#include "bumper.h"
#include "rlink.h"
#include <limits>

Bumper::Bumper(RLink& r, port::Name p) : Device(r), _port(r, p, (1 << PIN_LEFT) | (1 << PIN_RIGHT)) {}

Bumper::Reading Bumper::read() {
	Reading r;
	uint8_t pins = _port;

	r.left = pins & (1 << PIN_LEFT);
	r.right = pins & (1 << PIN_RIGHT);

	if(r.left && r.right)
		r.position = 0;
	else if(r.left)
		r.position = 1;
	else if(r.right)
		r.position = -1;
	else
		r.position = std::numeric_limits<float>::quiet_NaN();

	return r;
}