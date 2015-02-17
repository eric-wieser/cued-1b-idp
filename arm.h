#pragma once
#include "device.h"

class Arm : Device {
public:
	void up();
	void down();
	void open();
	void close();
};