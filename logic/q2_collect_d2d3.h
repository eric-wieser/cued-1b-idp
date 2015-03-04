#pragma once


#include "../robot.h"
#include "sequence.h"
#include "../eggtype.h"


class q2_collect_d2d3 : public StateMachine
{
private:
	EggType currentEgg;
	int conveyorPos;
public:
	q2_collect_d2d3(Robot& robot);

	virtual void _step();
};