#pragma once

#include <cstdint>
#include <chrono>

typedef unsigned long ticks_t;
typedef uint64_t timer_t;

namespace timing {

typedef std::chrono::steady_clock _timerClock;
extern ticks_t _ticks;

/** Increments the tick count.
*/
void tick();

timer_t now();
double diff(timer_t timer);

}

/** Returns an integer number of ticks.
*/
ticks_t ticks();