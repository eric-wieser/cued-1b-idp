#pragma once

#include "dev/rlink.h"
#include "dev/drive.h"
#include "dev/linesensors.h"
#include "dev/eggsensor.h"
#include "dev/courier.h"
#include "dev/arm.h"
#include "dev/ports.h"
#include "dev/bumper.h"
#include "eggtype.h"


struct Robot {
	Drive drive;
	Arm arm;
	LineSensors ls;
	EggSensor detector;
	Courier courier;

	Bumper bumper;

	Robot(RLink& rlink);
};

