

#include "timing.h"


ticks_t timing::_ticks = 0;


ticks_t ticks() {
	return timing::_ticks;
}

void timing::tick() {
	timing::_ticks++;
}