#include <cstdint>
#include "arm.h"
#include "ports.h"

const int PIN_UP_DOWNB = 0;
const int PIN_OPEN_CLOSEB = 1;

void Arm::up() {
	uint8_t word = _r.request(ports::read_instr(_port));
	word |= (1 << PIN_UP_DOWNB);
	_r.command(ports::write_instr(_port, word));
}
void Arm::down() {
	uint8_t word = _r.request(ports::read_instr(_port));
	word &= ~(1 << PIN_UP_DOWNB);
	_r.command(ports::write_instr(_port, word));
}
void Arm::open() {
	uint8_t word = _r.request(ports::read_instr(_port));
	word |= (1 << PIN_OPEN_CLOSEB);
	_r.command(ports::write_instr(_port, word));
}
void Arm::close() {
	uint8_t word = _r.request(ports::read_instr(_port));
	word &= ~(1 << PIN_OPEN_CLOSEB);
	_r.command(ports::write_instr(_port, word));
}
