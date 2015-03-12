#include "util/tracker.h"
#include "subroutines.h"

#include "util/logging.h"


bool shouldCollectD2D3(EggSensor::Reading& reading) {
	return reading.bestGuess == EGG_TASTY || reading.bestGuess == EGG_BROWN;
}

void q1_collect_d2d3(Robot& r) {
	Logger l("Q1: collect D2 + D3");

	goToConveyor(r, true);
	r.drive.stop();

	conveyorCollect(r, shouldCollectD2D3);
}
