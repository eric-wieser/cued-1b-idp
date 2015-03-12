
#include "util/tracker.h"
#include "subroutines.h"

#include "util/logging.h"

bool shouldCollectAny(EggSensor::Reading& reading)
{
	return reading.bestGuess != EGG_NONE;
}

void q4_collect_d1(Robot& r) {
	Logger l("Q4: collect D1 + rest");

	goToConveyor(r, false);
	r.drive.stop();

	conveyorCollect(r, shouldCollectAny);

	auto line = r.ls.read();

	if (line.state != LineSensors::Reading::JUNCTION) {
		l << "Reversing past junction to line up again" << std::endl;
		r.drive.straight(-0.2).wait();
		r.drive.move({forward: 1, steer: -0.2});

		do {
			line = r.ls.read();
		} while (!line.lsc);

		r.drive.stop();
	}
}
