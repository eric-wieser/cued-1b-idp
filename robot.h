#pragma once

#include "util/timing.h"
#include "dev/rlink.h"
#include "dev/drive.h"
#include "dev/linesensors.h"
#include "dev/eggsensor.h"
#include "dev/courier.h"
#include "dev/arm.h"
#include "dev/ports.h"
#include "eggtype.h"


struct Robot {
	Drive drive;
	Arm arm;
	LineSensors ls;
	EggSensor detector;
	Courier courier;

	Port limits;

	Robot(RLink& rlink);
};


namespace simple_robot {
}
