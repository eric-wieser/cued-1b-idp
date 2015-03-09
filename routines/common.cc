#include "robot.h"
#include "util/tracker.h"

#include "common.h"


void waitForLine(Robot& r, LineSensors::Reading::State s) {
	while(r.ls.read().state == s);
	while(r.ls.read().state != s);
}

void waitForLine(Robot& r, negate n) {
	while(r.ls.read().state != n.s);
	while(r.ls.read().state == n.s);
}