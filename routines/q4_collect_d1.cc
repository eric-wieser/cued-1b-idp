
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
}
