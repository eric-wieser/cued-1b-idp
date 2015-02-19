#pragma once
#include "device.h"
#include "ports.h"

class Arm : public Device {
private:
	Port _port;
public:
	enum {
		PIN_UP_DOWNB = 0,
		PIN_OPEN_CLOSEB = 1
	};
	void up();
	void down();
	void open();
	void close();

    Arm(RLink& r, port::Name name)
    	: Device(r), _port(r, name) {}
};