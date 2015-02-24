#include "robot.h"
#include "timing.h"
#include "dev/ports.h"

/*
Port mappings:

Port 0 - reserved for sensor test rig

Port 1 - line and egg sensors
	0  LSL  Line Sensor Left
	1  LSC  Line Sensor Centre
	2  LSR  Line Sensor Right
	3  LSA  Line Sensor Arm

	4  ELR	Egg LED Red
	5  ELG  Egg LED Green
	6  ELB  Egg LED Blue
	7  ---

Port 2 - arm
	0  APL  Arm Pivot Lowered
	1  APR  Arm Pivot Raised
	2  AGE  Arm Grab Extended
	3  AGR  Arm Grab Retracted

	4  APA  Arm Pivot Actuator
	5  AGA  Arm Grab Actuator
	6  ---
	7  BSF  Box Switch Front

Port 2 - courier indicator leds
	0  C1W  Courier bottom - white egg indicator LED
	1  C1B  Courier bottom - brown egg indicator LED
	2  C1T  Courier bottom - tasty egg indicator LED
	3  C2W  Courier middle - white egg indicator LED

	4  C2B  Courier middle - brown egg indicator LED
	5  C2T  Courier middle - tasty egg indicator LED
	6  C3B  Courier top - brown egg indicator LED
	7  C3B  Courier top - tasty egg indicator LED
*/
Robot::Robot(RLink& rlink) :
	drive(rlink, {
		radius: 0.005,
		spacing: 0.28,
		rpm: 20
	}),
	arm(rlink, port::P3),
	ls(rlink, port::P1),
	detector(rlink, port::P2) { }

const LineSensors::Reading& CachingRobot::ls() {
	if (_lsUpdated < ticks()) {
		_lsReading = Robot::ls.read();
	}

	return( _lsReading );
}


const EggSensor::Reading& CachingRobot::eld() {
	if (_esUpdated < ticks()) {
		_esReading = detector.read();
	}

	return( _esReading );
}
