
#include <cstdint>
#include <limits>

#include "ports.h"
#include "rlink.h"
#include "device.h"

#include "linesensors.h"


LineSensors::LineSensors(RLink& r, port::Name p)
		: Device(r), _port(r, p)
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

		// line lost - deduce loss direction from previous state
		case 0b000:
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

		// simple overlap of line, from leftmost to rightmost
		case 0b001: pos =  1.0f; break;
		case 0b011: pos =  0.5f; break;
		case 0b010: pos =  0.0f; break;
		case 0b110: pos = -0.5f; break;
		case 0b100: pos = -1.0f; break;

		// junction, but hit at an odd angle
		case 0b101:
			pos = _reading.position;
			ret.state = Reading::INVALID;
			break;

		//junction - assume position is unchanged
		case 0b111:
			pos = _reading.position;
			ret.state = Reading::JUNCTION;

			// Todo: Consider alternative cases.
			break;
	}

	ret.position = pos;
	_reading = ret;
	return( ret );
}
