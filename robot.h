#pragma once

#include "timing.h"
#include "drive.h"
#include "linesensors.h"
#include "eggsensor.h"
#include "arm.h"

class Robot
{
private:
	Drive _drive;

	LineSensors _lineSensors;
	LineSensors::Reading _lsReading;
	ticks_t _lsUpdated;

	EggSensor _eggSensor;
	EggSensor::Reading _esReading;
	ticks_t _esUpdated;

	Arm _arm;

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

	inline Arm& arm() { return _arm; }
};
