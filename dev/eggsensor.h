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
		PIN_LEDB = 5,
		PIN_LEDW = 6
	};
	struct Reading {
		float r;  /// red
		float b;  /// blue
		float w;  /// white
		float a;  /// ambient

		std::array<float, EGG_TYPE_COUNT> probabilities;
		EggType bestGuess;

		friend std::ostream& operator<< (std::ostream& stream, const Reading& matrix);
	};

	EggSensor(RLink& r, port::Name port);
	~EggSensor();

	Reading read();
};
