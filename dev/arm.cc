#include <cstdint>
#include "arm.h"
#include "ports.h"


Arm::Arm(RLink& r, port::Name name) :
	Device(r),
	_port(r, name)
{
	// TODO: the arm is a little dangerous... Do we want initialization to move it?
}

void Arm::down() {
	_port |= (1 << PIN_DOWN_UPB);
}
void Arm::up() {
	_port &= ~(1 << PIN_DOWN_UPB);
}
void Arm::close() {
	_port |= (1 << PIN_CLOSED_OPENB);
}
void Arm::open() {
	_port &= ~(1 << PIN_CLOSED_OPENB);
}
