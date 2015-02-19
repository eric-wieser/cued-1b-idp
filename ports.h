#pragma once
#include <cstdint>
#include <robot_instr.h>
#include "rlink.h"
#include "device.h"

namespace port {
	enum Name{
		P0,  P1,  P2,  P3,  P4,  P5,  P6,  P7,
		PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7
	};

	inline command_instruction write_instr(Name p) {
		if(p <= P7)
			return static_cast<command_instruction>(WRITE_PORT_0 + p);
		else
			return static_cast<command_instruction>(WRITE_PORT_A0 + p);
	}

	inline request_instruction read_instr(Name p) {
		if(p <= P7)
			return static_cast<request_instruction>(READ_PORT_0 + p);
		else
			return static_cast<request_instruction>(READ_PORT_A0 + p);
	}
}

class Port : public Device {
private:
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
			val |= ~_mask & _r.request(port::read_instr(_port));
		}
		_r.command(port::write_instr(_port), val);
	}


	inline Port& operator|=(uint8_t val) { *this = *this | val; return *this; }
	inline Port& operator&=(uint8_t val) { *this = *this & val; return *this; }
};
