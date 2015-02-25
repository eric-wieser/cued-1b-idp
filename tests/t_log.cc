

#include <iostream>

#include "../logging.h"


int main(int argc, char const *argv[])
{
	DEBUG(std::cout << "Debug" << std::endl);
	INFO(std::cout << "Info" << std::endl);
	WARN(std::cout << "Warn" << std::endl);
	ERROR(std::cout << "Error" << std::endl);

	return 0;
}