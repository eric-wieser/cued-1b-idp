#pragma once

#include "drive.h"
#include "linesensors.h"
#include "arm.h"
#include "eggsensor.h"

class Robot
{
private:
	RLink _rlink;
	Drive _drive;

	LineSensors _lineSensors;
	LineSensors::Reading _lsReading;
	ticks_t _lsUpdated;

	EggSensor _eggSensor;
	EggSensor::Reading _esReading;
	ticks_t _esUpdated;


public:
	Robot();
	~Robot();

	// Drive
	Drive& drive();

	// Line Sensors:
	const LineSensors::Reading& ls();
	inline bool lsl() { ls().lsl };
	inline bool lsc() { ls().lsc };
	inline bool lsr() { ls().lsr };
	inline bool lsa() { ls().lsa };
	inline float position() { ls().position };

	const EggSensor::Reading& eld();

	operator RLink();
};

extern Robot robot;
