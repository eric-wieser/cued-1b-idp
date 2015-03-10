#include "util/tracker.h"
#include "subroutines.h"

void goToConveyor(Robot& r) {
	r.drive.move({forward: 1.0f, steer: 1.0f});

	// wait till we leave the first line and arrive at the second
	waitForLine(r, negate { LineSensors::Reading::NONE });

	// wait till we leave the second line and arrive at the third
	waitForLine(r, negate { LineSensors::Reading::NONE });

	r.drive.move({forward: -1.0f, steer: 0.0f});

	// wait for the first junction
	waitForLine(r, LineSensors::Reading::JUNCTION);

	// wait for the second junction
	waitForLine(r, LineSensors::Reading::JUNCTION);

	// wait for the third junction
	waitForLine(r, LineSensors::Reading::JUNCTION);


	// turn
	r.drive.move({forward: 1.0f, steer: -1.0f});
	Timeout(std::chrono::duration<float>(180 / r.drive.maxSpeeds.rotary)).wait();
	Timeout(std::chrono::duration<float>(1)).wait();
}


void q1_collect_d2d3(Robot& r) {
	Drive::move_args args = {forward: 1, steer: -0.2};

	goToConveyor(r);
	r.drive.stop();

	for(int i  = 0; i < 5; i++) {
		r.drive.move(args);
		while(r.ls.read().lsl == 1);
		while(r.ls.read().lsl == 0);
		Tracker::instance()->logPose(-0.4 + 0.2 * i - 0.2, -0.8, 0);

		if(r.courier.volume() < 3) {
			r.drive.stop();
			delay(100);
			auto reading = r.detector.read();
			std::cout << "Egg at station " << i << ": " << reading.bestGuess << std::endl;

			if (reading.bestGuess == EGG_WHITE || reading.bestGuess == EGG_TASTY) {
				r.drive.straight(0.1).wait();
				r.drive.stop();

				r.arm.down();
				r.arm.close();
				r.arm.up();
				r.arm.open();

				r.courier.recordEggAdded(reading.bestGuess);
			}
		}
	}
	r.drive.stop();
}
