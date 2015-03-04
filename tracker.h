#pragma once

#include <fstream>

class Tracker {
private:
	static Tracker* _instance;
	std::ofstream _logFile;
	Tracker(const char* fname);

	template<typename T>
	void log(T data) {
		_logFile.write(reinterpret_cast<char const*>(&data), sizeof data);
		_logFile.flush();
	}

	struct __attribute__((__packed__)) p_lineSpot { char c; uint64_t t; };
	struct __attribute__((__packed__)) p_movement { char c; uint64_t t; float l; float r; };
	struct __attribute__((__packed__)) p_pose { char c; uint64_t t; float x; float y; float theta; };

public:
	static Tracker* instance();

	void logMovement(float left, float right);
	void logPose(float x, float y, float theta);
	void logLineSpot();
};
