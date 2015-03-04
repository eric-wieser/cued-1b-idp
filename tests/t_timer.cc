#include <iostream>
#include <chrono>
#include <cstdint>
#include <string>

#include "../util/timing.h"


int main() {
 
	timer_t MyTimer = timing::now();

	std::cout << "Timer started. ";
 
 	std::string s;
 	std::cin >> s;
 
	std::cout << timing::diff(MyTimer) << std::endl;
 
	return 0;
}