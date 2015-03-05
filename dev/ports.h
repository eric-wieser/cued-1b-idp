#pragma once
#include <cstdint>
#include <robot_instr.h>
#include "rlink.h"
#include "device.h"

namespace port {
	/// List of all the port names we could use
	enum Name{
		P0,  P1,  P2,  P3,  P4,  P5,  P6,  P7,
		PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
		NUM_PORTS
	};

	/// get the write instruction corresponding to a given port
	inline command_instruction write_instr(Name p) {
		if(p <= P7)
			return static_cast<command_instruction>(WRITE_PORT_0 + p);
		else
			return static_cast<command_instruction>(WRITE_PORT_A0 + p);
	}

	/// get the read instruction corresponding to a given port
	inline request_instruction read_instr(Name p) {
		if(p <= P7)
			return static_cast<request_instruction>(READ_PORT_0 + p);
		else
			return static_cast<request_instruction>(READ_PORT_A0 + p);
	}
}

/**
	Interface to a set on pins on a particular port.
	Allows masking of pins, to allow multiple `Device`s to share a I2C port
	without interfering with each other's bits

	Provides operator overloading for simple use:

		uint8_t reading = port;
		port = 0x42;

	Note that conversion to an int will return the current input, which is not
	necesarily the previous output
*/
class Port : public Device {
private:
	static uint8_t lastWrites[port::NUM_PORTS];
	port::Name _port;
	uint8_t _mask;
public:
	Port(RLink& r, port::Name p, uint8_t mask=0xFF) : Device(r), _port(p), _mask(mask) {}

	inline operator uint8_t() {
		uint8_t word = _r.request(port::read_instr(_port));
		return word & _mask;
	}
	inline void operator=(uint8_t val) {
		if(_mask != 0xFF) {
			// only bother reading if some bits are not masked
			val = (val & _mask) | (~_mask & lastWrites[_port]);
		}
		lastWrites[_port] = val;
		_r.command(port::write_instr(_port), val);
	}

	inline Port& operator|=(uint8_t val) { *this = *this | val; return *this; }
	inline Port& operator&=(uint8_t val) { *this = *this & val; return *this; }
};
