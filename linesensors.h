#pragma once

#include "device.h"

/**
Intended usage:

	LineSensors ls(robot);

	auto r = ls.read();

	if(r.state == r::LINE) {
		drive.move({
			.forward = 0.5,
			.steer=r.position * 0.2
		})
	}

*/
class LineSensors : public Device {
private:
	Port _port;
	bool _passive; // false => use LEDs
	Reading _reading;

public:
	struct Reading {
		enum { NONE, LINE, JUNCTION, INVALID } state;
		float position; // between -1 and 1, or +-Inf if no line, or NaN if invalid
		bool lsl, lsc, lsr, lsa;
	};

	LineSensors(RLink& r, port::Name p, bool passive = false);
	Reading read();
};
