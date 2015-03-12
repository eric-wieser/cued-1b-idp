#pragma once

#define LEVEL_DEBUG 1
#define LEVEL_INFO 2
#define LEVEL_WARN 3
#define LEVEL_ERROR 4

#ifndef LOGGING
	#define LOGGING LEVEL_WARN
#endif

#if LOGGING <= LEVEL_DEBUG
	#define DEBUG(x) (x)
#else
	#define DEBUG(x) 
#endif

#if LOGGING <= LEVEL_INFO
	#define INFO(x) (x)
#else
	#define INFO(x) 
#endif

#if LOGGING <= LEVEL_WARN
	#define WARN(x) (x)
#else
	#define WARN(x) 
#endif

#if LOGGING <= LEVEL_ERROR
	#define ERROR(x) (x)
#else
	#define ERROR(x) 
#endif

#include <string>
#include <iostream>

class Robot;

class Logger {
public:
	Logger* const parent;
private:
	static Logger* _active;
	std::string _indent;
public:
	Logger(std::string name, Logger* _parent = NULL);

	Logger(const Logger& other) = delete;
	Logger(Logger&& other);

	~Logger() {
		if(std::uncaught_exception())
			*this << "[threw]" << std::endl;
		else
			*this << "[done]" << std::endl;
		_active = this->parent;
	}

	Logger child(std::string name) {
		return Logger(name, this);
	}

	void checkpoint(Robot& r, std::string id) {
		// r.drive.stop();
		*this << "Checkpoint: " << id << std::endl;
		// std::cin.get();
	}

	inline int depth() const {
		return parent == NULL ? 1 : 1 + parent->depth();
	}

	static Logger& active() {
		if(_active == NULL) {
			_active = new Logger("Unnamed logger");
		}
		return *_active;
	}

	template<typename T>
	friend std::ostream& operator<< (Logger& logger, const T& t);
};


template<typename T>
std::ostream& operator <<(Logger& logger, const T& t) {
	return std::cout << logger._indent << t;
}
