#pragma once
#include "rlink.h"

/** Base class for all devices */

class Device {
protected:
	RLink* _r;
	Device(RLink* r) : _r(r) {};
};
