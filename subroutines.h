#include "robot.h"



void goToJunction(Robot& r, float distance);
void turnAtJunction(Robot& r);
void goToConveyor(Robot& r);

struct negate { LineSensors::Reading::State s; };
void waitForLine(Robot& r, LineSensors::Reading::State s);
void waitForLine(Robot& r, negate n);

namespace demos {
	void f1(Robot& r);
	void f2(Robot& r);
	void f3(Robot& r);
	void all(Robot& r);
}