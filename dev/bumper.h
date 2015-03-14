#pragma once

#include "device.h"
#include "ports.h"

/**
	Interface to the limit switch bumper on the front of the robot
*/
class Bumper : public Device {
private:
	Port _port;
	enum {
		PIN_LEFT = 0,
		PIN_RIGHT = 1
	};
public:
	struct Reading {
		bool left;      ///< left switch is pressed
		bool right;     ///< right switch is pressed
		float position; ///< 1 for left, -1 for right, 0 for straight, and NaN for not pressed
	};

	Bumper(RLink& r, port::Name port);

	Reading read();
};
