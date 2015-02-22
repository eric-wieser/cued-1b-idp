#pragma once

#include <array>

#include "eggtype.h"
#include "device.h"
#include "rlink.h"

#include "ports.h"
#include <array>

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
class EggSensor : public Device {
private:
	Port _port;

public:
	enum {
		PIN_LEDR = 0,
		PIN_LEDG = 1,
		PIN_LEDB = 2
	};
	struct Reading {
		std::array<float, 3> probabilities;
		EggType bestGuess;

		float r;
		float g;
		float b;
		float a; /// ambient
	};

	Reading read();
	EggSensor(RLink& r, port::Name port)
		: Device(r), _port(r, port, 0x7) {};
};
