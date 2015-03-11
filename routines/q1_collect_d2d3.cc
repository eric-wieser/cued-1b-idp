#include "util/tracker.h"
#include "subroutines.h"


bool shouldCollectD2D3(EggSensor::Reading& reading)
{
	return( (reading.bestGuess == EGG_TASTY)
			|| (reading.bestGuess == EGG_WHITE));
}

void q1_collect_d2d3(Robot& r) {
	goToConveyor(r, true);
	r.drive.stop();

	conveyorCollect(r, shouldCollectD2D3);
}
