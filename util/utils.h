#pragma once

#include <chrono>
#include "robot_delay.h"

inline void delay(const std::chrono::milliseconds& d) {
	delay(d.count());
}

#define ALL(v) v.begin(), v.end()
