#include "robot.h"



void goToJunction(Robot& r, float distance);
void turnAtJunction(Robot& r);
void goToConveyor(Robot& r);

struct negate { LineSensors::Reading::State s; };
void waitForLine(Robot& r, LineSensors::Reading::State s);
void waitForLine(Robot& r, negate n);