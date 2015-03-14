#pragma once

#include <array>

#include "device.h"
#include "../eggtype.h"
#include "ports.h"
#include <iostream>

/**
	Interface to the LEDs and LDR comprising the egg sensor

	Includes the algorithm for identifying eggs
*/
class EggSensor : public Device {
private:
	Port _port;
	enum {
		PIN_LEDR = 4,
		PIN_LEDB = 5,
		PIN_LEDW = 6
	};

public:
	struct Reading {
		uint8_t r;  ///< reflection from red LED
		uint8_t b;  ///< reflection from blue LED
		uint8_t w;  ///< reflection from white LED
		uint8_t a;  ///< ambient reading

		/// "distances" to each egg. Lower values indicate greater likelihood
		std::array<float, EGG_TYPE_COUNT> probabilities;

		/// shorthand for most likely egg type
		EggType bestGuess;

		friend std::ostream& operator<< (std::ostream& stream, const Reading& reading);
	};

	EggSensor(RLink& r, port::Name port);
	~EggSensor();

	/// read the sensor, taking an average over multiple samples
	Reading read(int samples = 5);
};
