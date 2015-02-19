#pragma once

#include "device.h"

/**
Intended usage:

	LineSensor ls(robot);

	auto r = ls.read();

	if(r.state == r::LINE) {
		drive.move({
			.forward = 0.5,
			.steer=r.position * 0.2
		})
	}

*/
class LineSensor : public Device {
private:
	bool _passive; // false => use LEDs

public:
	struct Reading {
		enum { NONE, LINE, JUNCTION, INVALID } state;
		float position; // between -1 and 1, or NaN if no line
	};

	Reading read();
	LineSensor(RLink& r, bool passive = false) : Device(r), _passive(passive);
}
