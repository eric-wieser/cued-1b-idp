

#include "drive.h"
#include "linesensors.h"
#include "eggsensor.h"
#include "timing.h"

#include "robot.h"


Robot robot;


Robot::Robot() {
	std::cout << "constructed" << std::endl;
	_rlink.initialise();
	std::cout << "initialised" << std::endl;

	// Initialise Sensors/motors
	_drive = Drive();
	_lineSensors = LineSensors();

}


Robot::~Robot() {
	// Stop drive?
}


const LineSensors::Reading& ls() {
	if (_lsUpdated < ticks()) {
		_lsReading = _lineSensors.read();
	}

	return( _lsReading )
}


const EggSensor::Reading& eld() {
	if (_esUpdated < ticks()) {
		_esReading = _eggSensor.read();
	}

	return( _esReading );
}

operator RLink() {
	return _rlink;
}