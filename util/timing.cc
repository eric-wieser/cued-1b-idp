

#include "timing.h"


t_ticks timing::_ticks = 0;


t_ticks ticks() {
	return timing::_ticks;
}

void timing::tick() {
	timing::_ticks++;
}

 
t_timer timing::now() {
    _timerClock::time_point t1 = _timerClock::now();
 
	return t_timer(std::chrono::duration_cast<std::chrono::nanoseconds>
			(t1.time_since_epoch()).count());
}
 
double timing::diff(t_timer start) {
	_timerClock::time_point t1 = _timerClock::now();
 
	return double(t_timer(std::chrono::duration_cast<std::chrono::nanoseconds>
			(t1.time_since_epoch()).count()) - start) / 1000000.0;
}
