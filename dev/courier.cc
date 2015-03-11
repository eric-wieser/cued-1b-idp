#include <cstdint>
#include <cassert>
#include "courier.h"
#include "ports.h"
#include "robot_delay.h"
#include "drive.h"
#include "linesensors.h"


static const float holdSpeed = -0.5;
static const float dropSpeed = 1;
static const float liftSpeed = -1;

Courier::Courier(RLink& r, port::Name ledPort, port::Name lightGatePort) :
		Device(r),
		_ledPort(r, ledPort, 0b111111),
		_lightGatePort(r, lightGatePort, 1 << PIN_LIGHTGATE),
		_volume(0)
{
	_contents[0] = _contents[1] = _contents[2] = EGG_NONE;
	_lightGatePort = 0xff;
	r.command(MOTOR_3_GO, Drive::convertSpeed(holdSpeed));

	_updateLeds();
}

void Courier::_updateLeds() {
	uint8_t mask = 0xFF;

	switch(_contents[2]) {
		case EGG_WHITE: mask &= ~0b000001; break;
		case EGG_BROWN: mask &= ~0b000010; break;
		case EGG_TASTY: mask &= ~0b000011; break;
	}


	switch(_contents[1]) {
		case EGG_WHITE: mask &= ~0b000100; break;
		case EGG_BROWN: mask &= ~0b001000; break;
		case EGG_TASTY: mask &= ~0b001100; break;
	}

	switch(_contents[0]) {
		case EGG_WHITE: mask &= ~0b010000; break;
		case EGG_BROWN: mask &= ~0b100000; break;
		case EGG_TASTY: mask &= ~0b110000; break;
	}

	_ledPort = mask;
}

void Courier::recordEggAdded(EggType egg_t) {
	assert(_volume < 3);

	_contents[_volume++] = egg_t;
	_updateLeds();
}

void Courier::unloadEgg() {
	assert(_volume != 0);

	// move the motor. 
	_r.command(MOTOR_3_GO, Drive::convertSpeed(dropSpeed));
	delay(1000);
	_r.command(MOTOR_3_GO, Drive::convertSpeed(liftSpeed));

	// update internal state, if we succeed
	_contents[0] = _contents[1];
	_contents[1] = _contents[2];
	_contents[2] = EGG_NONE;
	_volume--;
	_updateLeds();

	delay(2000);
	_r.command(MOTOR_3_GO, Drive::convertSpeed(holdSpeed));

}

int Courier::volume() const { return _volume; }
EggType Courier::egg(int n) const { return _contents[n]; }
bool Courier::eggDetected() const { return uint8_t(_lightGatePort) == 0; }

// bool Courier::eggDetected() const { return false; }
