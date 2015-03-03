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
		PIN_LEDR = 4,
		PIN_LEDG = 5,
		PIN_LEDW = 6
	};
	struct Reading {
		std::array<float, EGG_TYPE_COUNT> probabilities;
		EggType bestGuess;

		float r;
		float g;
		float w;
		float a; /// ambient

		friend std::ostream& operator<< (std::ostream& stream, const Reading& matrix);
	};

	Reading read();
	EggSensor(RLink& r, port::Name port)
		: Device(r), _port(r, port, (1 << PIN_LEDR) | (1 << PIN_LEDG) | (1 << PIN_LEDW)) {};
};
