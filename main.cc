
#include "robot.h"
#include "timing.h"

int main() {
	for (;;) {
		timing::tick();

		robotLogic();
	}

	return(0);
}
