#pragma once

#include "common.h"
#include "linefollow.h"
#include "../robot.h"

/** [Q1] Collect D2/3 Eggs
Start: Starting position
End: Last position on conveyor

\verbatim embed:rst
.. image:: flowcharts/q1_collect_d2d3.svg
\endverbatim
*/
void q1_collect_d2d3(Robot& r);

/** [Q2] Deliver D2/3 Eggs
Start: Last position on conveyor
End: Junction between D2/3 boxes
*/
void q2_deliver_d2d3(Robot& r);

/** [Q3] Return from D2/3 Boxes
Start: Junction between D2/3 boxes
End: Starting square (at centre junction, facing west)
*/
void q3_return_from_d2d3(Robot& r);

/** [Q4] Collect D1 Eggs
Start: Starting square (at centre junction, facing west)
End: Last position on conveyor
*/
void q4_collect_d1(Robot& r);

/** [Q5] Deliver D1 Eggs
Start: Last position on conveyor
End: D1 box
*/
void q5_deliver_d1(Robot& r);

/** [Q6] Return from d1
Start: D1 box
End: Within starting area
*/
void q6_return_from_d1(Robot& r);
