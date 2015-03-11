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


/** Follows a line to the next junction
@param r A reference to an initialised Robot
@param distance The approximate distance to the junction
@throw LineLost Line was lost and couldn't be recovered.
@throw Timeout::Expired if it wasn't reached in the expected time based on distance.
*/


typedef bool linefollowTerminator(Robot&, const LineSensors::Reading&);
extern linefollowTerminator until_junction;
extern linefollowTerminator until_switch;
extern linefollowTerminator until_xjunction;
extern linefollowTerminator forever;

void goToJunction(Robot& r, float distance, linefollowTerminator* terminator = &until_junction);


/** Turns the robot at a junction
@param r A reference to an initialised Robot
@param turns The number of 90 degree turns to go through, counter-clockwise being
		positive.
@throw LineLost The target line couldn't be found, recovery failed.
*/
void turnAtJunction(Robot& r, int turns, bool goForward = true);
