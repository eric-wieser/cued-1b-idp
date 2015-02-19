

#include "timing.h"
#include "port.h"
#include "drive.h"
#include "linesensors.h"
#include "eggsensor.h"
#include "arm.h"

class RLink;

#include "robot.h"


Robot::Robot(Rlink& rlink)
		: _drive(rlink), 
			_lineSensors(rlink, port::P1), 
			_eggSensor(rlink, port::P2),
			_arm(rlink, port::P3) {
}


Robot::~Robot() {
	// Stop drive?
}


const LineSensors::Reading& Robot::ls() {
	if (_lsUpdated < ticks()) {
		_lsReading = _lineSensors.read();
	}

	return( _lsReading )
}


const EggSensor::Reading& Robot::eld() {
	if (_esUpdated < ticks()) {
		_esReading = _eggSensor.read();
	}

	return( _esReading );
}