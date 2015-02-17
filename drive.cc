#include "drive.h"
#include "<cstdint>"

/// convert floating point speed to sign/magnitude
static uint8_t convert_speed(float s) {
	uint8_t res = min(abs(s), 1) * 127;

	if(s < 0) res |= 0x80;

	return res
}

Drive::move(move_args args) {
	float left = args.forward - args.steer
	float right = args.forward + args.steer;
	return setWheelSpeed(left, right);
}

Drive::setWheelSpeed(float left, float right) {
	// apply any sign corrections here
	uint8_t left_i = convert_speed(left);
	uint8_t right_i = convert_speed(right);

	if (left_i == right_i) {
		_r.command(BOTH_MOTORS_GO_SAME, left_i);
	}
	else if (left_i == -right_i) {
		// TODO: should this be right_i?
		_r.command(BOTH_MOTORS_GO_OPPOSITE, left_i);
	}
	else {
		_r.command(MOTOR_1_GO, left_i);
		_r.command(MOTOR_2_GO, right_i);
	}
}

Drive::~Drive() {
	stop();
}
