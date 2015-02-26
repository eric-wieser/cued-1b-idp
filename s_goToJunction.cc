#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <chrono>
#include "util/utils.h"
#include "util/timeout.h"
#include "robot.h"

using namespace std::chrono;

/// Exception thrown if the line is lost
struct LineLost : public std::exception {
	const LineSensors::Reading lastReading;  ///< Reading that indicated a lost line
	const float distanceLeft;                ///< distance left along the line when lost

	LineLost(LineSensors::Reading r, float d) : lastReading(r), distanceLeft(d) {};

	/// override of std::exception::what
	virtual const char* what() const throw() {
		if(lastReading.position > 0)
			return "lost to the left";
		if(lastReading.position < 0)
			return "lost to the right";
		else
			return "lost to the rear";
	};
};

struct HardwareDamaged : public std::exception {};

/**
	@param distance  estimated travel distance, in m

	@throws Timeout::Expired  Took too long to get to junction
	@throws LineLost          Line lost for too long
	@throws HardwareDamaged   Line state invalid for too long
	                          Middle sensor broken? Wheels jammed?
*/

void goToJunction_inner(Robot& r, float distance) {
	std::deque<LineSensors::Reading::State> history;
	Drive d = r.drive;


	// Predict the time taken to get there, with a 10% margin
	duration<float> tPredicted(distance / r.drive.maxSpeeds.linear * 1.1f);
	Timeout timeout = tPredicted;

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
		if(nJunc >= 3)
			return;

		// if the last 5 readings have been no line, we've failed
		if(nNone == 5)
			throw LineLost(line, timeout.remaining() / tPredicted * distance);

		// if the last 5 readings have been invalid...
		if(nInvalid == 5)
			throw HardwareDamaged();

		// only keep the last 5 readings
		if(history.size() > 5)
			history.pop_back();

		timeout.check();
		delay(milliseconds(10));
	}
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

void goToJunction(Robot& r, float distance) {
	while(1) {
		// try to follow the line to the appropriate distance
		try{
			goToJunction_inner(r, distance);
			return;
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
}
