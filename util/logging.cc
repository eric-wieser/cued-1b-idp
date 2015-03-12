#include "logging.h"

Logger* Logger::_active;

Logger::Logger(std::string name, Logger* _parent ) : parent(_parent), _indent(std::string(depth(), '\t')) {
	if(_parent != NULL)
		*_parent << name << "(" << depth() << "):" << std::endl;
	else
		std::cout << name << ":" << std::endl;

	_active = this;
}

Logger::Logger(Logger&& other) : parent(other.parent) {
	if(&other == _active) _active = this;
}