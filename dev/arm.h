#pragma once
#include "device.h"
#include "ports.h"

class Arm : public Device {
private:
	Port _port;
public:
	enum {
		PIN_DOWN_UPB = 6,
		PIN_CLOSED_OPENB = 7
	};
	void up();
	void down();
	void open();
	void close();

    Arm(RLink& r, port::Name name)
    	: Device(r), _port(r, name) {}
};