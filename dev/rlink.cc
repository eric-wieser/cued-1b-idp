#include "rlink.h"

void RLink::initialise() {
	#ifdef __arm__
		bool ok = robot_link::initialise("127.0.0.1");
	#else
		bool ok = robot_link::initialise(5); // team no
	#endif

	if(!ok) throw LinkError(*this);
}
void RLink::command(command_instruction cmd, int arg) {
	bool ok = robot_link::command(cmd, arg);
	if(!ok) throw LinkError(*this);
}
int RLink::request(request_instruction req) {
	int res = robot_link::request(req);
	if(res == REQUEST_ERROR) throw LinkError(*this);
	return res;
}
uint8_t RLink::status() {
	int res = robot_link::request(STATUS);
	// error while checking for an error? It's a comms error - bit 0
	if(res == REQUEST_ERROR) return 1;

	return res;
}
