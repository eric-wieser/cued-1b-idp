#include "robot.h"
#include "ports.h"
#include "timing.h"


Robot::Robot(RLink& rlink)
		: _drive(rlink),
			_lineSensors(rlink, port::P1),
			_eggSensor(rlink, port::P2),
			_arm(rlink, port::P3) {
}


Robot::~Robot() {
	// nothing to do here - all the subcomponents do clean up anyway
}


const LineSensors::Reading& Robot::ls() {
	if (_lsUpdated < ticks()) {
		_lsReading = _lineSensors.read();
	}

	return( _lsReading );
}


const EggSensor::Reading& Robot::eld() {
	if (_esUpdated < ticks()) {
		_esReading = _eggSensor.read();
	}

	return( _esReading );
}
