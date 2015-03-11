
#include "util/tracker.h"
#include "subroutines.h"


bool shouldCollectAny(EggSensor::Reading& reading)
{
	return( reading.bestGuess != EGG_NONE );
}


void q4_collect_d1(Robot& r) {
	goToConveyor(r, false);
	r.drive.stop();

	conveyorCollect(r, shouldCollectAny);

	auto line = r.ls.read();

	if (line.state != LineSensors::Reading::JUNCTION) {
		r.drive.straight(-0.2).wait();
		r.drive.move({forward: 1, steer: -0.2});

		do {
			line = r.ls.read();
		} while (!line.lsc);

		r.drive.stop();
	}
}
