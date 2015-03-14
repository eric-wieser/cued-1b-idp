#pragma once

#include <chrono>
#include "robot_delay.h"

/**
	Class for keeping track of expected times for operations

	Example usage:

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
			std::cout << "took too long" << std::endl;
		}

*/
class Timeout {
private:
	typedef std::chrono::system_clock clock;
	clock::time_point _end;

public:
	typedef std::chrono::duration<float> duration_type;
	class Expired : std::exception {};

	/// create a timeout `duration` in the future
	Timeout(duration_type duration) :
		_end(
			clock::now() + std::chrono::duration_cast<clock::time_point::duration>(duration)
		) {}

	/// create a timeout ending at `end`
	Timeout(clock::time_point end) : _end(end) {}

	inline duration_type remaining() const { return _end - clock::now(); }
	inline bool hasexpired() const { return remaining() <= 0; }

	/// check if the timeout has expired, and throw Expired if so
	inline void check() const {
		if(hasexpired())
			throw Expired();
	}

	/// wait for the timeout to expire
	inline void wait() const {
		delay(std::chrono::duration_cast<std::chrono::milliseconds>(
			 remaining()
		).count());
	}

	inline Timeout operator+(duration_type d) const {
		return Timeout(_end + std::chrono::duration_cast<clock::time_point::duration>(d));
	}
	inline Timeout operator-(duration_type d) const {
		return Timeout(_end - std::chrono::duration_cast<clock::time_point::duration>(d));
	}
};
