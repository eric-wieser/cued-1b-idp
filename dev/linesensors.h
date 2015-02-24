#pragma once

#include "ports.h"
#include "device.h"
#include "rlink.h"

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
public:
	struct Reading {
		enum State { NONE, LINE, JUNCTION, INVALID };
		State state;
		float position; // between -1 and 1, or +-Inf if no line, or NaN if invalid
		bool lsl, lsc, lsr, lsa;
	};

private:
	Port _port;
	Reading _reading;

public:
	LineSensors(RLink& r, port::Name p);
	Reading read();
};
