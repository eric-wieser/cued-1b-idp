#pragma once

#include <array>

#include "device.h"
#include "../eggtype.h"
#include "ports.h"
#include <iostream>

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
		PIN_LEDW = 2
	};
	struct Reading {
		std::array<float, 3> probabilities;
		EggType bestGuess;

		float r;
		float g;
		float w;
		float a; /// ambient

		friend std::ostream& operator<< (std::ostream& stream, const Reading& matrix);
	};

	Reading read();
	EggSensor(RLink& r, port::Name port)
		: Device(r), _port(r, port, 0x7) {};
};
