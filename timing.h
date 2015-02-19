#pragma once

typedef unsigned long ticks_t;

namespace timing {

extern ticks_t _ticks;

/** Increments the tick count.
*/
void tick();

}

/** Returns an integer number of ticks.
*/
ticks_t ticks();