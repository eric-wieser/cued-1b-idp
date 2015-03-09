/**
Class for timeouts

eg:
	using namespace std::literals::chrono_literals;

	Timeout timeout(2s);
	try {
		doAThing();
		timeout.check();
		do {
			keepGoing();
			timeout.check();
		} while (stillGoing())
	} catch(Timeout::Expired) {
		
	}
*/
#pragma once

#include <chrono>
#include "robot_delay.h"

class Timeout {
private:
	typedef std::chrono::system_clock clock;
	clock::time_point _end;

public:
	typedef std::chrono::duration<float> duration_type;
	class Expired {};

	// chrono really is a mess to use
	Timeout(duration_type duration) :
		_end(
			clock::now() + std::chrono::duration_cast<clock::time_point::duration>(duration)
		) {}

	Timeout(clock::time_point end) : _end(end) {}

	inline void check() const {
		if(clock::now() >= _end)
			throw Expired();
	}

	inline bool hasexpired() const {
		return clock::now() >= _end;
	}

	inline duration_type remaining() const {
		return _end - clock::now();
	}

	inline void wait() const {
		delay(std::chrono::duration_cast<std::chrono::milliseconds>(
			_end - clock::now()
		).count());
	}

	inline Timeout operator+(duration_type d) const {
		return Timeout(_end + std::chrono::duration_cast<clock::time_point::duration>(d));
	}
	inline Timeout operator-(duration_type d) const {
		return Timeout(_end - std::chrono::duration_cast<clock::time_point::duration>(d));
	}
};
