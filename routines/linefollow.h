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

enum GOTOJUNC_RET {
	RET_JUNCTION,
	RET_TIMEOUT
};

GOTOJUNC_RET goToJunction(Robot& r, float distance);
void turnAtJunction(Robot& r, int turns);