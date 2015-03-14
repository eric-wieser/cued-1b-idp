#pragma once

#include <string>
#include <iostream>

class Robot;

/**
	Heirarchical logger, use to produce indented logs
*/
class Logger {
public:
	Logger* const parent;
private:
	static Logger* _active;
	std::string _indent;
public:
	Logger(std::string name, Logger* _parent = NULL);

	// move constructor only, to preserve `_active`
	Logger(const Logger& other) = delete;
	Logger(Logger&& other);

	/// upon destruction, log either "[done]" or "[threw]"
	~Logger() {
		if(std::uncaught_exception())
			*this << "[threw]" << std::endl;
		else
			*this << "[done]" << std::endl;
		_active = this->parent;
	}

	/// create a sublogger of this logger
	Logger child(std::string name) {
		return Logger(name, this);
	}

	/// record a checkpoint. Scope for stopping the robot and waiting for user interaction
	void checkpoint(Robot& r, std::string id) {
		// r.drive.stop();
		*this << "Checkpoint: " << id << std::endl;
		// std::cin.get();
	}

	/// the depth of this logger - used for indentation
	inline int depth() const {
		return parent == NULL ? 1 : 1 + parent->depth();
	}

	/// get the current active logger
	static Logger& active() {
		if(_active == NULL) {
			_active = new Logger("Unnamed logger");
		}
		return *_active;
	}

	template<typename T>
	friend std::ostream& operator<< (Logger& logger, const T& t);
};

/// Output content to the logger, prefixed with appropriate indentation
template<typename T>
std::ostream& operator <<(Logger& logger, const T& t) {
	return std::cout << logger._indent << t;
}
