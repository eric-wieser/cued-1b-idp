#include "robot.h"
#include "util/tracker.h"
#include "dev/courier.h"

#include "common.h"


void waitForLine(Robot& r, LineSensors::Reading::State s) {
	while(r.ls.read().state == s);
	while(r.ls.read().state != s);
}


void waitForLine(Robot& r, negate n) {
	while(r.ls.read().state != n.s);
	while(r.ls.read().state == n.s);
}


void goToConveyor(Robot& r, bool east)
{
	r.drive.move({forward: 1.0f, steer: (east ? 1.0f : -1.0f)});

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

	// turn 90 degrees
	r.drive.move({forward: 0.5f, steer: -0.5f});
	Timeout(r.drive.timeForTurn(90 + 10, 0.5)).wait();
}


void checkpoint(Robot& r, std::string id) {
	// r.drive.stop();
	std::cout << "Checkpoint: " << id << std::endl;
	// std::cin.get();
}


void conveyorCollect(Robot& r, EGG_CALLBACK shouldCollect)
{
	Drive::move_args args = {forward: 1, steer: -0.2};

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

			if (shouldCollect(reading)) {
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

Timeout::duration_type driveToBumper(Robot& r) {
	using namespace std::chrono;
	Drive d = r.drive;

	d.move({forward: 0.5f, steer: 0});

	auto t = system_clock::now();
	while(true) {
		auto bump = r.bumper.read();
		if(std::isfinite(bump.position)) break;
	}
	auto ret = duration_cast<Timeout::duration_type>(system_clock::now() - t);
	// try for one second
	Timeout straightness_time(Timeout::duration_type(1.0f));
	while(true) {
		auto bump = r.bumper.read();
		if(std::isfinite(bump.position)) {
			d.move({forward: 0.5f, steer: bump.position*0.7f});
		}
		else {
			d.move({forward: 0.5f, steer: 0});
		}

		// timeout or straight
		if(straightness_time.hasexpired())
			break;
		if(bump.position == 0)
			break;
	}

	return ret;
}

void dropEggs(Robot& r, int n) {
	// inch forward until the limit switch is hit
	auto returnTime = driveToBumper(r);


	for(int i = 0; i < n; i++) {
		if(i != 0) {
			delay(500); // to allow the previous egg to be removed
		}
		std::cout << "Dropping egg " << r.courier.egg(0) << std::endl;

		// Check light gate
		if (!r.courier.eggDetected()) {
			// Wobble

			r.drive.straight(-0.06f).wait();

			auto line = r.ls.read();
			for (int i = 0; i < 5 && !r.courier.eggDetected(); i++) {
				r.drive.turn(10).wait();
				Timeout t = r.drive.turn(-15);
				do {
					line = r.ls.read();
				} while (!line.lsc && !t.hasexpired());
				r.drive.stop();
			}

			// Return to box TODO: TIMEOUT
			driveToBumper(r);
		}
		r.courier.unloadEgg();
	}

	// undo the straight motion (TODO: match time)
	r.drive.move({forward: -0.5, steer: 0});
	Timeout(returnTime).wait();
}
