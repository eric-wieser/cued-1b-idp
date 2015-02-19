#pragma once

#include "eggtype.h"
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
class EggSensor : public Device {
private:
	port::Name _port;

public:
	struct Reading {
		float[3] probabilities;
		EggType bestGuess;

		float r;
		float g;
		float b;
		float a; /// ambient
	};

	Reading read();
	EggSensor(RLink& r, port::Name port)
		: Device(r), _port(port);
}
