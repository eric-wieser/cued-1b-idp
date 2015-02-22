

#include <iostream>
#include "../timing.h"

int main(int argc, char const *argv[])
{
	std::cout << ticks() << std::endl;

	timing::tick();

	std::cout << ticks() << std::endl;

	return 0;
}