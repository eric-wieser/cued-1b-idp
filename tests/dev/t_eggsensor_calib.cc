#include "dev/rlink.h"
#include "dev/eggsensor.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>
#include "robot_delay.h"

int main() {
	RLink r;
	std::cout << "constructed" << std::endl;
	r.initialise();
	std::cout << "initialised" << std::endl;

	EggSensor es(r, port::P1);

	std::ofstream f("eggdata.csv");

	f << "egg,r,g,w,a" << std::endl;

	for(const char* name : {"brown", "white", "cream", "none"}) {
		char c;
		std::cout << name << std::endl;
		while((c = getchar()) != '\n' && c != EOF);


		for(int i = 0; i < 20; i++) {
			auto r = es.read();
			f << name <<","<< r.r <<","<< r.g <<","<< r.w <<","<< r.a << std::endl;
			delay(250);
		}
	}
	return 0;
}