#pragma once

#include "dev/linesensors.h"
#include "robot.h"

typedef bool (*EGG_CALLBACK)(EggSensor::Reading& reading);

struct negate { LineSensors::Reading::State s; };
void waitForLine(Robot& r, LineSensors::Reading::State s);
void waitForLine(Robot& r, negate n);
void goToConveyor(Robot& r, bool east = true);
void conveyorCollect(Robot& r, EGG_CALLBACK shouldCollect);
void checkpoint(Robot& r, std::string id);
