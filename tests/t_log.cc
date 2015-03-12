

#include <iostream>

#include "util/logging.h"


int main(int argc, char const *argv[])
{
	DEBUG(std::cout << "Debug" << std::endl);
	INFO(std::cout << "Info" << std::endl);
	WARN(std::cout << "Warn" << std::endl);
	ERROR(std::cout << "Error" << std::endl);

	Logger l("Section 1");
	l << "Hi" << std::endl;
	{
		Logger l2 = l.child("SubSection 1");
		l2 << "Hi" << std::endl;
	}
	{
		Logger l2 = l.child("SubSection 2");
		l2 << "Hi" << std::endl;
	}
	{
		Logger l2 = Logger::active().child("SubSection 3");
		l2 << "Hi3" << std::endl;
	}


	return 0;
}