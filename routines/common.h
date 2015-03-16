#pragma once

#include "dev/linesensors.h"
#include "robot.h"


struct negate { LineSensors::Reading::State s; };
void waitForLine(Robot& r, LineSensors::Reading::State s);
void waitForLine(Robot& r, negate n);

void goToConveyor(Robot& r, bool east = true);

typedef bool (*EGG_CALLBACK)(EggSensor::Reading& reading);

/** Drives along the conveyor collecting eggs

\verbatim embed:rst
.. image:: flowcharts/conveyorCollect.svg

.. graphviz:: flowcharts/conveyorCollect.dot

\endverbatim


*/
void conveyorCollect(Robot& r, EGG_CALLBACK shouldCollect);

void dropEggs(Robot& r, int n = 1);
