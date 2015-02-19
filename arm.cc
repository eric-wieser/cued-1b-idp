#include <cstdint>
#include "arm.h"
#include "ports.h"

void Arm::up() {
	_port |= (1 << PIN_UP_DOWNB);
}
void Arm::down() {
	_port &= ~(1 << PIN_UP_DOWNB);
}
void Arm::open() {
	_port |= (1 << PIN_OPEN_CLOSEB);
}
void Arm::close() {
	_port &= ~(1 << PIN_OPEN_CLOSEB);
}
