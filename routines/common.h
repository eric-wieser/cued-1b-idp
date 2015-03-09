#pragma once

#include "dev/linesensors.h"
#include "robot.h"

void dropCreme(Robot& r);
void dropWhite(Robot& r);

struct negate { LineSensors::Reading::State s; };
void waitForLine(Robot& r, LineSensors::Reading::State s);
void waitForLine(Robot& r, negate n);