#include "dev/rlink.h"
#include "dev/drive.h"
#include <cstdint>
#include <iostream>


#ifdef __arm__
	#error "ncurses library is not available on the robot"
#endif

namespace nc {
#include <ncurses.h>
}

Drive::move_args charToDir(int c) {
	float f = 1;
	float t = 1;
	switch(c) {
		case '8': return { f,  0};
		case '9': return { f, -t};
		case '6': return { 0, -t};
		case '3': return {-f, -t};
		case '2': return {-f,  0};
		case '1': return {-f,  t};
		case '4': return { 0,  t};
		case '7': return { f,  t};
		default:  return { 0,  0};
	}
}

int main() {
	RLink r;
	std::cout << "constructed" << std::endl;
	r.initialise();
	std::cout << "initialised" << std::endl;

	Drive d(r);

	// ncurses stuff
	nc::newterm(NULL, stdin, stdout);
	nc::noecho();
	nc::cbreak();
	nc::keypad(nc::stdscr, true);

	while(1)
	{
		auto dir = charToDir(nc::wgetch(nc::stdscr));
		std::cout << "driving: forward=" << dir.forward
		          << ", steer=" << dir.steer << "\r";
		std::cout.flush();
		d.move(dir);
	}
	return 0;
}
