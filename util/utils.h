#pragma once

#include <chrono>
#include "robot_delay.h"

using namespace std::literals::chrono_literals;

inline void delay(const std::chrono::milliseconds& d) {
	delay(d.count());
}

#define ALL(v) std::begin(v), std::end(v)
