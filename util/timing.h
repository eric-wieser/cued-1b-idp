#pragma once

#include <cstdint>
#include <chrono>

typedef unsigned long t_ticks;
typedef uint64_t t_timer;

namespace timing {

typedef std::chrono::system_clock _timerClock;
extern t_ticks _ticks;

/** Increments the tick count.
*/
void tick();

t_timer now();
double diff(t_timer timer);

}

/** Returns an integer number of ticks.
*/
t_ticks ticks();
