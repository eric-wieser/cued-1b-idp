#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <chrono>
#include "util/utils.h"
#include "util/timeout.h"
#include "robot.h"

#include "linefollow.h"

using namespace std::chrono;

/// override of std::exception::what
const char* LineLost::what() const throw() {
	if(lastReading.position > 0)
		return "lost to the left";
	if(lastReading.position < 0)
		return "lost to the right";
	else
		return "lost to the rear";
};


/**
	@param distance  estimated travel distance, in m

	@throws LineLost          Line lost for too long
	@throws HardwareDamaged   Line state invalid for too long
	                          Middle sensor broken? Wheels jammed?
*/
GOTOJUNC_RET goToJunction_inner(Robot& r, float distance) {
	std::deque<LineSensors::Reading::State> history;
	Drive d = r.drive;

	// Predict the time taken to get there, with a 10% margin
	duration<float> tPredicted(distance / r.drive.maxSpeeds.linear * 1.1f);
	Timeout timeout = tPredicted;

	// assume we started on a junction
	bool atJunction = true;

	while(1) {
		// read the line sensors
		auto line = r.ls.read();
		history.push_front(line.state);

		// if we know where the line is, adjust our course
		if(std::isfinite(line.position)) {
			d.move({
				forward: 1.0f,
				steer: 0.5f * line.position
			});
		}
		else {
			// otherwise, we've lost the line
			// inch forward slowly, in case it was a bad reading
			d.move({forward: 0.5f});
		}

		// look back over the history, to deal with bad readings
		int nJunc = std::count(ALL(history), LineSensors::Reading::JUNCTION);
		int nNone = std::count(ALL(history), LineSensors::Reading::NONE);
		int nInvalid = std::count(ALL(history), LineSensors::Reading::INVALID);

		// if 3 of the last 5 readings have been junctions, we're done
		if(!atJunction && nJunc >= 3)
			break;

		// if we started at a junction, and we're no longer on one, clear the flag
		if(line.state != LineSensors::Reading::JUNCTION)
			atJunction = false;

		// if the last 5 readings have been no line, we've failed
		if(nNone == 5)
			throw LineLost(line, timeout.remaining() / tPredicted * distance);

		// if the last 5 readings have been invalid...
		if(nInvalid == 5)
			throw HardwareDamaged();

		// only keep the last 5 readings
		if(history.size() > 5)
			history.pop_back();

		if (timeout.hasexpired()) {
			return(RET_TIMEOUT);
		}

		delay(milliseconds(10));
	}

	return(RET_JUNCTION);
}

class NoLineFound : public std::exception {};

void reFindLine(Robot& r, float lastPos) {
	const float reverseDist = 0.05; //m
	const float reverseSpeed = 0.5;

	const float steerAngle = 45;
	const float steerSpeed = 0.5;


	float sign = copysignf(1, lastPos);

	// lost the line behind us
	if(sign == 0) {
		// reverse until we find a line
		try {
			Drive d = r.drive;
			Timeout timeout = d.straight(-reverseDist, reverseSpeed);
			while(r.ls.read().state != LineSensors::Reading::LINE) {
				timeout.check();
			}

			return;
		}
		catch(Timeout::Expired) {}

		// pick a side for further searching
		sign = 1;
	}

	// turn in the direction we think we lost the line
	try {
		Drive d = r.drive;
		Timeout timeout = d.turn(sign*steerAngle, steerSpeed);
		while(r.ls.read().state != LineSensors::Reading::LINE) {
			timeout.check();
		}

		return;
	}
	catch(Timeout::Expired) {}

	// turn in the other direction
	try {
		Drive d = r.drive;
		Timeout timeout = d.turn(-2*sign*steerAngle, steerSpeed);
		while(r.ls.read().state != LineSensors::Reading::LINE) {
			timeout.check();
		}

		return;
	}
	catch(Timeout::Expired) {}


	throw NoLineFound();
}


GOTOJUNC_RET goToJunction(Robot& r, float distance) {
	GOTOJUNC_RET ret;

	while(1) {
		// try to follow the line to the appropriate distance
		try{
			ret = goToJunction_inner(r, distance);
			break;
		}
		catch(LineLost& lost) {
			std::cout << "Lost: " << lost.what() << ", " << lost.distanceLeft << "m remain" << std::endl;
			distance = lost.distanceLeft;
			try {
				reFindLine(r, lost.lastReading.position);
			}
			catch (NoLineFound) {
				std::cout << "Could not refind" << std::endl;
				throw lost;
			}
		}
	}

	return(ret);
}

void turnAtJunction(Robot& r, bool left) {
#ifdef ZOOLANDER
	left = false;
#endif

	// Past Line - True if in order to turn we should cross 2 lines
	// according to the linefollower return.
	bool pastLine = false;

	try {
			auto ret = goToJunction_inner(r, 0.2);

			if (ret == RET_JUNCTION) {
				// Maybe throw an error?
			}
	}
	catch (LineLost& lost) {
		r.drive.straight(lost.distanceLeft).wait();

		pastLine = (left == (lost.lastReading.position < 0.0f));
	}

	{
		Drive d = r.drive;
		auto t = d.turn(left ? 90 : -90);
		t.add(std::chrono::duration<float>(0.5));

		int state = pastLine ? 1 : 0;
		while (state != 3) {
			auto line = r.ls.read();

			if (t.hasexpired()) {
				state = 4;
			}

			switch (state) {
				case 0: // Start, past line
					if (line.lsc)
						state = 1;
					break;
				case 1: // On first line
					if (!line.lsc)
						state = 2;
					break;
				case 2: // Past 1st line
					if (line.lsc)
						state = 3;
					break;
				case 3: // On 2nd line
					d.stop();
					break;
				case 4: // probably gone too far?
					t = d.turn(left ? -30 : 30);
					state = 5;
					break;
				case 5: // Gone to far, sweeping back
					if (line.lsc) 
						state = 3;
					else if (t.hasexpired()) {
						t = d.turn(left ? 60 : 60);
						state = 6;
					}
					break;
				case 6: // Gone to far, sweep forwards again?
					if (line.lsc) 
						state = 3;
					else if (t.hasexpired())
						throw LineLost(line, 0);
					break;
			}
		}
	}


}