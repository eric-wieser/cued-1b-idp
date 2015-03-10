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
		bool lsl;  ///< left sensor reading
		bool lsc;  ///< right sensor reading
		bool lsr;  ///< center sensor reading
		bool lsa;  ///< arm sensor reading

		enum State { NONE, LINE, JUNCTION, INVALID };
		State state;

		/**
			Line position, where between -1 and 1, with left positive.
			+-Inf and NaN indicate a lost line
		*/
		float position;
	};

private:
	Port _port;
	Reading _reading;

public:
	LineSensors(RLink& r, port::Name p);
	Reading read();
};

inline std::ostream& operator <<(std::ostream& stream, const LineSensors::Reading& r) {

	return stream
		<< "{position: " << r.position
		<< ", sensors: ["
		<< (r.lsl ? '#' : ' ') << " "
		<< (r.lsc ? '#' : ' ') << " "
		<< (r.lsr ? '#' : ' ') << "]}";
}
