#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <chrono>
#include "util/utils.h"
#include "util/timeout.h"
#include "util/logging.h"
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


bool until_junction(Robot&, const LineSensors::Reading& ls) {
	return ls.state == LineSensors::Reading::JUNCTION;
}

bool until_switch(Robot& r, const LineSensors::Reading&) {
	return std::isfinite(r.bumper.read().position);
}

bool until_xjunction(Robot&, const LineSensors::Reading& ls) {
	return ls.state == LineSensors::Reading::JUNCTION && ls.lsa;
}

bool forever(Robot&, const LineSensors::Reading& ls) {
	return false;
}




void followUntil_once(Robot& r, float distance, linefollowTerminator* terminator) {
	std::deque<LineSensors::Reading::State> history;
	std::deque<bool> terminateHistory;

	Drive d = r.drive;

	// Predict the time taken to get there
	duration<float> tPredicted = r.drive.timeForStraight(distance);

	// add a 25% margin before timing out for non-timed code
	if(terminator != NULL) tPredicted = tPredicted * 1.25 + duration<float>(0.25);

	Timeout timeout = tPredicted;

	// assume we started on a junction
	bool terminateCleared = false;

	while(1) {
		// read the line sensors
		auto line = r.ls.read();
		history.push_front(line.state);
		terminateHistory.push_front(terminator ? terminator(r, line) : false);

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
		int nTerminated = std::count(ALL(terminateHistory), true);

		int nNone = std::count(ALL(history), LineSensors::Reading::NONE);
		int nInvalid = std::count(ALL(history), LineSensors::Reading::INVALID);

		// if 3 of the last 5 readings have been junctions, we're done
		if(terminateCleared && nTerminated >= 3)
			return;

		// if we started at a junction, and we're no longer on one, clear the flag
		if(nTerminated == 0)
			terminateCleared = true;

		// if the last 5 readings have been no line, we've failed
		if(nNone == 5)
			throw LineLost(line, timeout.remaining() / tPredicted * distance);

		// // if the last 5 readings have been invalid...
		// if(nInvalid == 5)
		// 	throw HardwareDamaged();

		// only keep the last 5 readings
		if(history.size() > 5) {
			history.pop_back();
			terminateHistory.pop_back();
		}


		// if a terminator is specified, throw if we reatch a timeout first. Otherwise, return
		if(terminator != NULL)
			timeout.check();
		else if(timeout.hasexpired())
			return;

		delay(milliseconds(10));
	}
}


void reFindLine(Robot& r, float lastPos) {
	const float reverseDist = 0.05; //m
	const float reverseSpeed = 0.5;

	const float steerAngle = 45;
	const float steerSpeed = 0.5;

	float sign = copysignf(1, lastPos);

	Drive d = r.drive;

	// lost the line behind us
	if(sign == 0) {
		// reverse until we find a line
		Timeout timeout = d.straight(-reverseDist, reverseSpeed);
		while(!timeout.hasexpired()) {
			if(r.ls.read().state == LineSensors::Reading::LINE) return;
		}

		// pick a side for further searching
		sign = 1;
	}
	{
		// turn in the direction we think we lost the line
		Timeout timeout = d.turn(sign*steerAngle, steerSpeed);
		while(!timeout.hasexpired()) {
			if(r.ls.read().state == LineSensors::Reading::LINE) return;
		}
	}
	{
		// turn in the other direction
		Timeout timeout = d.turn(-2*sign*steerAngle, steerSpeed);
		while(!timeout.hasexpired()) {
			if(r.ls.read().state == LineSensors::Reading::LINE) return;
		}
	}
	throw NoLineFound();
}


void followUntil(Robot& r, float distance, linefollowTerminator* term) {
	Logger::active() << "Following line for " << distance << "m..." << std::endl;
	while(1) {
		// try to follow the line to the appropriate distance
		try{
			return followUntil_once(r, distance, term);
		}
		catch(LineLost& lost) {
			Logger::active() << "Lost: " << lost.what() << ", " << lost.distanceLeft << "m remain" << std::endl;
			distance = lost.distanceLeft;
			try {
				reFindLine(r, lost.lastReading.position);
			}
			catch (NoLineFound) {
				Logger::active() << "Could not refind" << std::endl;
				throw lost;
			}
		}
	}
}

void turnAtJunction(Robot& r, int turns, bool goForward) {
	int sign = (turns < 0) ? -1 : 1;

	Drive d = r.drive;


	if(goForward) {
		try {
			Logger::active() << "Driving over junction" << std::endl;
			followUntil_once(r, 0.17, NULL);
		}
		catch (LineLost& lost) {
			Logger::active() << "Lost line at junction - continueing" << std::endl;
			r.drive.straight(lost.distanceLeft).wait();
		}
	}

	Logger::active() << "Turning " << (turns * 90) << "deg" << std::endl;

	Timeout real_t = d.turn(turns * 90);
	Timeout early_t = real_t - d.timeForTurn(sign*30);
	Timeout late_t = real_t + d.timeForTurn(sign*30);

	while(!early_t.hasexpired());

	while(!late_t.hasexpired()) {
		auto line = r.ls.read();
		if(line.state != LineSensors::Reading::NONE) return;
	}

	reFindLine(r, sign);
}
