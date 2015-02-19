#pragma once
#include "device.h"

class Arm : public Device {
public:
	void up();
	void down();
	void open();
	void close();

    Arm(RLink* r) : Device(r) {}
};