#pragma once


#include "../robot.h"
#include "sequence.h"
#include "../eggtype.h"


class q2_collect_d2d3 : public StateMachine
{
public:
	q1_(Robot& robot);

	virtual void _step();
};
