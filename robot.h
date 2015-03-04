#pragma once

#include "timing.h"
#include "dev/rlink.h"
#include "dev/drive.h"
#include "dev/linesensors.h"
#include "dev/eggsensor.h"
#include "dev/arm.h"
#include "eggtype.h"

struct Robot {
	Drive drive;
	Arm arm;
	LineSensors ls;
	EggSensor detector;
	int eggsLoaded;
	EggType[3] eggTypes;

	Robot(RLink& rlink);
};

class CachingRobot : private Robot
{
private:
	LineSensors::Reading _lsReading;
	t_ticks _lsUpdated;
	EggSensor::Reading _esReading;
	t_ticks _esUpdated;

public:
	CachingRobot(RLink& rlink) : Robot(rlink) { };

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

	inline Arm& arm() { return Robot::arm; }
};

namespace simple_robot {
}