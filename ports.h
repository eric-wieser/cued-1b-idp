#pragma once
#include <robot_link.h>

namespace port {
	enum {
		P0,  P1,  P2,  P3,  P4,  P5,  P6,  P7,
		PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7
	} Name;

	request_instruction write_instr(Name p) {
		if(p <= P7)
			return WRITE_PORT_0 + p;
		else
			return WRITE_PORT_A0 + p;
	}

	request_instruction read_instr(Name p) {
		if(p <= P7)
			return READ_PORT_0 + p;
		else
			return READ_PORT_A0 + p;
	}
}