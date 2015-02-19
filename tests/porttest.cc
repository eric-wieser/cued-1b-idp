#include "../ports.h"
#include "../rlink.h"
#include <cstdint>
#include <iostream>

int main() {
	RLink r;
	std::cout << "constructed" << std::endl;
	r.initialise();
	std::cout << "initialised" << std::endl;

	Port upper(r, port::P0, 0x0F);
	Port lower(r, port::P0, 0xF0);

	int i = 0;
	while(true) {
		std::cout << "lower:" << std::endl;
		std::cout << "  old:" << (int) uint8_t(lower) << std::endl;
		std::cout << "  new:";
		std::cin >> i;
		lower = i;
		std::cout << "  res:" << (int) uint8_t(lower) << std::endl;
		std::cout << std::endl<< std::endl;


		std::cout << "upper:" << std::endl;
		std::cout << "  old:" << (int) uint8_t(upper) << std::endl;
		std::cout << "  new:";
		std::cin >> i;
		upper = i;
		std::cout << "  res:" << (int) uint8_t(upper) << std::endl;
		std::cout << std::endl<< std::endl;
	}
}