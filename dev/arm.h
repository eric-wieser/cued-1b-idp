#pragma once

#include "device.h"
#include "ports.h"

/**
	Interface to the egg-grabbing arm of the robot
*/
class Arm : public Device {
private:
	Port _port;
public:
	enum {
		PIN_DOWN_UPB = 6,
		PIN_CLOSED_OPENB = 7
	};
	Arm(RLink& r, port::Name name);

	void up();
	void down();
	void open();
	void close();
};