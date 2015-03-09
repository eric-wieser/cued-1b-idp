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
	class Expired {};

	// chrono really is a mess to use
	Timeout(std::chrono::duration<float> duration) :
		_end(
			clock::now() + std::chrono::duration_cast<clock::time_point::duration>(duration)
		) {}

	inline void check() {
		if(clock::now() >= _end)
			throw Expired();
	}

	inline bool hasexpired() {
		return( clock::now() >= _end );
	}

	inline std::chrono::duration<float> remaining() {
		return _end - clock::now();
	}

	inline void wait() {
		delay(std::chrono::duration_cast<std::chrono::milliseconds>(
			_end - clock::now()
		).count());
	}
};
