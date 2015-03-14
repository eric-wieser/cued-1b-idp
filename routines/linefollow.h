#pragma once

struct LineLost : public std::exception {
	const LineSensors::Reading lastReading;  ///< Reading that indicated a lost line
	const float distanceLeft;                ///< distance left along the line when lost

	LineLost(LineSensors::Reading r, float d) : lastReading(r), distanceLeft(d) {};

	/// override of std::exception::what
	virtual const char* what() const throw();
};


struct NoLineFound : public std::exception {};
struct HardwareDamaged : public std::exception {};

typedef bool linefollowTerminator(Robot&, const LineSensors::Reading&);
extern linefollowTerminator until_junction;
extern linefollowTerminator until_switch;
extern linefollowTerminator until_xjunction;
extern linefollowTerminator forever;

/**
	Follow a line until an event

	@param r           A reference to a Robot
	@param distance    The expected distance to drive, in meters, before the event
	                   occurs
	@param terminator  A function taking ``(Robot&, const LineSensors::Reading&)``
	                   that returns true when the robot should stop.
	                   If NULL, stop after `distance` has been travelled

	@throw LineLost          The target line couldn't be found, recovery failed.
	@throw Timeout::Expired  It's taking more than 25% longer than expected to
	                         reach the target

	For example:

		followUntil(robot, 0.6, until_junction)   // follow the line to a junction 60cm away
		followUntil(robot, 0.2, until_xjunction)  // follow the line to a cross-junction 20cm away
		followUntil(robot, 0.2, until_bumper)     // follow the line until the bumper is pressed 20cm away
		followUntil(robot, 0.2, NULL)             // follow the line for exactly 20cm
*/
void followUntil(Robot& r, float distance, linefollowTerminator* terminator = &until_junction);


/**
	Turns the robot at a junction
	@param r          A reference to a  Robot
	@param turns      The number of 90 degree turns to go through,
	                  counter-clockwise being positive.
	@param goForward  `false` if the robot already has its wheelbase over the junction

	@throw LineLost The target line couldn't be found, recovery failed.
*/
void turnAtJunction(Robot& r, int turns, bool goForward = true);
