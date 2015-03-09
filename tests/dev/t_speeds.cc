#include "robot.h"
#include "routines/subroutines.h"
#include "util/tracker.h"

#include <cstdint>
#include <iostream>
#include <robot_delay.h>
#include <chrono>

std::chrono::milliseconds timeLines(Robot& r, int n) {
	using namespace std::chrono;

	int i = 0;
	while(r.ls.read().state == LineSensors::Reading::NONE);
	auto start = steady_clock::now();
	for(int i = 0; i < n; i++) {
		// wait till we leave the current line
		while(r.ls.read().state != LineSensors::Reading::NONE);
		// wait till we arrive at the next line
		while(r.ls.read().state == LineSensors::Reading::NONE);
	}
	return duration_cast<milliseconds>(steady_clock::now() - start);

}

int main() {
	RLink link;
	std::cout << "constructed" << std::endl;
	link.initialise();
	std::cout << "initialised" << std::endl;

	while(1) {
		Robot r(link);



		std::cout
			<< "Choose a speed test:" << std::endl
			<< " 1 - drive across 3 lines on plateau" << std::endl
			<< " 2 - rotate at intersection" << std::endl;

		std::cout << "Your choice: ";
		int n = -1;
		std::cin >> n;

		if(n == 1) {
			r.drive.move({1, 0});
			auto t = timeLines(r, 2);
			r.drive.stop();
			std::cout << "Distance: 1.68m, Time: " << t.count() << "ms" << std::endl;
			std::cout << "Speed: " << 1.68 * 1000 / t.count() << std::endl;
		}
		if(n == 2) {
			r.drive.move({0, 1});
			auto t = timeLines(r, 8);
			r.drive.stop();
			std::cout << "Distance: 720 degrees, Time: " << t.count() << "ms" << std::endl;
			std::cout << "Speed: " << 720 * 1000 / t.count() << std::endl;
		}

		if(n == -1) break;
	}
}