#include "tracker.h"
#include <chrono>

Tracker::Tracker(const char* s) : _logFile(s) { }

Tracker* Tracker::instance() {
	if(!_instance) _instance = new Tracker("tracker.dat");
	return _instance;
}

static uint64_t get_time() {
	auto now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

void Tracker::logMovement(float left, float right) {
	log((p_movement) {'M', get_time(), left, right});
}

void Tracker::logPose(float x, float y, float theta) {
	log((p_pose) {'P', get_time(), x, y, theta});
}

void Tracker::logLineSpot() {
	log((p_lineSpot) {'L', get_time()});
}


Tracker* Tracker::_instance = NULL;
