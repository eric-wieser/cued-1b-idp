

#include "timing.h"


ticks_t timing::_ticks = 0;


ticks_t ticks() {
	return timing::_ticks;
}

void timing::tick() {
	timing::_ticks++;
}

 
timer_t timing::now() {
    _timerClock::time_point t1 = _timerClock::now();
 
	return timer_t(std::chrono::duration_cast<std::chrono::nanoseconds>
			(t1.time_since_epoch()).count());
}
 
double timing::diff(timer_t start) {
	_timerClock::time_point t1 = _timerClock::now();
 
	return double(timer_t(std::chrono::duration_cast<std::chrono::nanoseconds>
			(t1.time_since_epoch()).count()) - start) / 1000000.0;
}