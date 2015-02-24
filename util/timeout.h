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

class Timeout {
private:
	typedef std::chrono::system_clock clock;
	clock::time_point _end;
public:
	class Expired {};

	// chrono really is a mess to use
	template<class Rep, class Period>
	Timeout(std::chrono::duration<Rep,Period> duration) :
		_end(
			std::chrono::time_point_cast<clock::time_point::duration>(
				clock::now() + duration
			)
		) {}

	inline void check() {
		if(clock::now() >= _end)
			throw Expired();
	}
};
