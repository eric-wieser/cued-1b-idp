#include <cstdint>
#include <cassert>
#include "courier.h"
#include "ports.h"
#include "robot_delay.h"
#include "drive.h"


Courier::Courier(RLink& r, port::Name ledPort)
		: Device(r), _ledPort(r, ledPort), _volume(0)
{
	r.command(MOTOR_3_GO, Drive::convertSpeed(-0.1));
}

void Courier::_updateLeds() {
	uint8_t mask = 0;

	switch(_contents[0]) {
		case EGG_WHITE: mask |= 0b000001; break;
		case EGG_BROWN: mask |= 0b000010; break;
		case EGG_TASTY: mask |= 0b000011; break;
	}


	switch(_contents[1]) {
		case EGG_WHITE: mask |= 0b000100; break;
		case EGG_BROWN: mask |= 0b001000; break;
		case EGG_TASTY: mask |= 0b001100; break;
	}

	switch(_contents[2]) {
		case EGG_WHITE: mask |= 0b010000; break;
		case EGG_BROWN: mask |= 0b100000; break;
		case EGG_TASTY: mask |= 0b110000; break;
	}

	_ledPort = mask;
}

void Courier::recordEggAdded(EggType egg_t) {
	assert(_volume < 3);

	_contents[++_volume] = egg_t;
	_updateLeds();
}

void Courier::unloadEgg() {
	assert(_volume != 0);

	// move the motor. TODO: use light gate
	_r.command(MOTOR_3_GO, Drive::convertSpeed(0.5));
	delay(1000);
	_r.command(MOTOR_3_GO, 0);
	delay(1000);
	_r.command(MOTOR_3_GO, Drive::convertSpeed(-0.5));
	delay(1000);
	_r.command(MOTOR_3_GO, 0);

	// update internal state, if we succeed
	_contents[0] = _contents[1];
	_contents[1] = _contents[2];
	_contents[2] = EGG_NONE;
	_updateLeds();
}

int Courier::volume() { return _volume; }