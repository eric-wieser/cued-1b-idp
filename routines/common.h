#pragma once

#include "dev/linesensors.h"
#include "robot.h"


struct negate { LineSensors::Reading::State s; };
void waitForLine(Robot& r, LineSensors::Reading::State s);
void waitForLine(Robot& r, negate n);

void goToConveyor(Robot& r, bool east = true);

typedef bool (*EGG_CALLBACK)(EggSensor::Reading& reading);

/** Drives along the conveyor collecting eggs
\htmlonly
<div align='center'>
<embed src="http://1b-idp.readthedocs.org/en/latest/_static/conveyorCollect.svg" type="image/svg+xml"     pluginspage="http://www.adobe.com/svg/viewer/install/" />
</div>
\endhtmlonly 
*/
void conveyorCollect(Robot& r, EGG_CALLBACK shouldCollect);

void checkpoint(Robot& r, std::string id);
void dropEggs(Robot& r, int n = 1);
