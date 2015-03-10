#pragma once
#include "rlink.h"

/** Base class for all devices which require a link to the robot */
class Device {
protected:
	/// internal reference to a robot connection
	RLink& _r;
	Device(RLink& r) : _r(r) {};
};
