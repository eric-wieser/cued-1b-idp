#include "robot.h"

void turnAtJunction(Robot& r) {
	{
		Drive d = r.drive;
		d.straight(0.2).wait();
	}
	{
		Drive d = r.drive;
		d.turn(90).wait();
	}
}
