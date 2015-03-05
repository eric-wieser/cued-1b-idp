#include <cstdint>
#include "arm.h"
#include "ports.h"

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
