
#include <cstdint>
#include <limits>

#include "ports.h"
#include "rlink.h"
#include "device.h"

#include "linesensors.h"


LineSensors::LineSensors(RLink& r, port::Name p, bool passive)
		: Device(r), _passive(passive), _port(r, p) 
{
}

LineSensors::Reading LineSensors::read() {
	LineSensors::Reading ret;

	// Read from I2C
	uint8_t sensors = _port;
	ret.lsl = sensors & 0x1;
	ret.lsc = sensors & 0x2;
	ret.lsr = sensors & 0x4;
	ret.lsa = sensors & 0x8;

	// Determine position
	float pos = 0.0f;
	ret.state = Reading::LINE;
	switch (sensors & 0b111) {
		case 0: // 000
			if (_reading.position < 0.0f) {
				pos = -std::numeric_limits<float>::infinity();
			}
			else if (_reading.position > 0.0f) {
				pos = std::numeric_limits<float>::infinity();
			}
			else {
				pos = std::numeric_limits<float>::quiet_NaN();
			}
			ret.state = Reading::NONE;

			break;

		case 3: // 011
			pos = 0.5f;
			break;
		case 1: // 001
			pos = 1.0f;
			break;

		case 2: // 010
			pos = 0.0f;
			break;

		case 6: // 110
			pos = -0.5f;
			break;
		case 4: // 100
			pos = -1.0f;
			break;

		case 5: // 101
			// Theoretically possible at junctions.
			pos = _reading.position;
			ret.state = Reading::INVALID;
			break;

		case 7: // 111
			// Junction. 
			pos = _reading.position;
			ret.state = Reading::JUNCTION;

			// Todo: Consider alternative cases.
			break;
	}

	ret.position = pos;
	_reading = ret;
	return( ret );
}
