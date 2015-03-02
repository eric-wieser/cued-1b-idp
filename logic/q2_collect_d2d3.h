#pragma once


#include "../robot.h"
#include "sequence.h"


class q2_collect_d2d3 : public StateMachine
{
public:
	q2_collect_d2d3(Robot& robot);

	virtual void _step();
};