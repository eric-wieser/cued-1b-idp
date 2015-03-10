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

	std::cout << "Try and vary the ambient light while sampling" << std::endl;

	const char* names[EGG_TYPE_COUNT] = {"brown", "white", "cream", "none"};

	for(int i = 0; i < EGG_TYPE_COUNT; i++) {
		auto name = names[i];
		char c;
		std::cout << "Place over " << name << " egg, and hit enter" << std::endl;
		while((c = getchar()) != '\n' && c != EOF);


		for(int i = 0; i < 20; i++) {
			auto r = es.read();
			f << name <<","<< int(r.r) <<","<< int(r.b) <<","<< int(r.w) <<","<< int(r.a) << std::endl;
			delay(250);
		}
	}
	return 0;
}