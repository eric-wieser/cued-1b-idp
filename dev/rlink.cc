#include "rlink.h"

void RLink::initialise() {
	#ifdef __arm__
		bool ok = robot_link::initialise();
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
