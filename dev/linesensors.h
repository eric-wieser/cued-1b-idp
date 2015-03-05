#pragma once

#include "ports.h"
#include "device.h"
#include "rlink.h"

/**
	Interface to the three front-mounted line sensors
*/
class LineSensors : public Device {
public:
	struct Reading {
		bool lsl;  /// left
		bool lsc;  /// right
		bool lsr;  /// center
		bool lsa;  /// arm

		enum State { NONE, LINE, JUNCTION, INVALID };
		State state;

		float position; // between -1 and 1, or +-Inf if no line, or NaN if invalid
	};

private:
	Port _port;
	Reading _reading;

public:
	LineSensors(RLink& r, port::Name p);
	Reading read();
};
