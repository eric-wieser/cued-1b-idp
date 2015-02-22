#pragma once

#include "dev/rlink.h"
#include "dev/drive.h"
#include "dev/linesensors.h"
#include "dev/eggsensor.h"
#include "dev/arm.h"

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
	Robot(RLink& rlink);
	~Robot();

	// Drive
	Drive& drive();

	// Line Sensors:
	const LineSensors::Reading& ls();
	inline bool lsl() { return ls().lsl; };
	inline bool lsc() { return ls().lsc; };
	inline bool lsr() { return ls().lsr; };
	inline bool lsa() { return ls().lsa; };
	inline float position() { return ls().position; };

	const EggSensor::Reading& eld();

	inline Arm& arm() { return _arm; }
};
