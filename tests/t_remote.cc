#include "dev/rlink.h"
#include "robot.h"
#include <cstdint>
#include <iostream>


#ifdef __arm__
	#error "ncurses library is not available on the robot"
#endif

namespace nc {
#include <ncurses.h>
}

int main() {

	std::cout
		<< "      Numpad buttons                      robot actions      " << std::endl
		<< "+-----+-----+-----+-----+           +-----+-----+-----+-----+" << std::endl
		<< "|     |     |     |     |           |     |     |     |     |" << std::endl
		<< "|     |  /  |  *  |  -  |           |     | arm | arm | arm |" << std::endl
		<< "|     |     |     |     |           |     | open| grab| up  |" << std::endl
		<< "+-----+-----+-----+-----+           +-----+-----+-----+-----+" << std::endl
		<< "|     |     |     |     |           | <-  |  ^  |  -> |     |" << std::endl
		<< "|  7  |  8  |  9  |     |           |   | |  |  | |   |     |" << std::endl
		<< "|     |     |     |     |           | . | |  |  | | . |     |" << std::endl
		<< "|-----+-----+-----+  +  |           |-----+-----+-----+ arm |" << std::endl
		<< "|     |     |     |     |           |   ^ |     | ^   | down|" << std::endl
		<< "|  4  |  5  |  6  |     |    ==>    | | | |stop | | | |     |" << std::endl
		<< "|     |     |     |     |           | V   |     |   V |     |" << std::endl
		<< "+-----+-----+-----------+           +-----+-----+-----------+" << std::endl
		<< "|     |     |     |     |           | | . |  |  | . | |     |" << std::endl
		<< "|  1  |  2  |  3  |     |           | |   |  |  |   | |     |" << std::endl
		<< "|     |     |     |     |           |  -> |  V  | <-  |     |" << std::endl
		<< "|-----+-----+-----+enter|           |-----+-----+-----+ drop|" << std::endl
		<< "|           |     |     |           |           |     | egg |" << std::endl
		<< "|  0        |  .  |     |           | read line | read|     |" << std::endl
		<< "|           |     |     |           |  sensors  | egg |     |" << std::endl
		<< "+-----------+-----+-----+           +-----------+-----+-----+" << std::endl;

	RLink r;
	std::cout << "constructed" << std::endl;
	r.initialise();
	std::cout << "initialised" << std::endl;
	Robot robot(r);

	// ncurses stuff
	auto sc = nc::newterm(NULL, stdin, stdout);
	nc::noecho();
	nc::cbreak();
	nc::keypad(nc::stdscr, true);

	try {
		while(1)
		{
			int c = nc::wgetch(nc::stdscr);

			float f = 1, t = 1;

			switch(c) {
				case '8': robot.drive.move({ f,  0}); break;
				case '9': robot.drive.move({ f, -t}); break;
				case '6': robot.drive.move({ 0, -t}); break;
				case '3': robot.drive.move({-f, -t}); break;
				case '2': robot.drive.move({-f,  0}); break;
				case '1': robot.drive.move({-f,  t}); break;
				case '4': robot.drive.move({ 0,  t}); break;
				case '7': robot.drive.move({ f,  t}); break;
				case '[': robot.drive.turn(90).wait(); robot.drive.stop(); break;
				case ']': robot.drive.turn(-90).wait(); robot.drive.stop(); break;
				default:  robot.drive.move({ 0,  0}); break;
			}

			switch(c) {
				case '+': robot.arm.down();  break;
				case '-': robot.arm.up();    break;
				case '/': robot.arm.open();  break;
				case '*': robot.arm.close(); break;

				case '.': {
						auto egg = robot.detector.read().bestGuess;
						std::cout << "Egg type: " << egg << "\r\n";
						if(robot.courier.volume() < 3) {
							robot.courier.recordEggAdded(egg);
						}
						else {
							std::cout << "Too many eggs" << std::endl;
						}
					}
					break;
				case '0':
					std::cout << "Line position: " << robot.ls.read().position << "\r\n";
					break;

				case '\n':
					if(robot.courier.eggDetected()) {
						std::cout << "Egg detected" << std::endl;
					}
					if(robot.courier.volume() == 0) {
						robot.courier.recordEggAdded(EGG_BROWN);
						std::cout << "Adding dummy egg" << std::endl;
					}
					robot.courier.unloadEgg();
					break;
			}

			std::cout.flush();
		}
	}
	catch(...) {
		nc::delscreen(sc);
		nc::endwin();
		throw;
	}

	nc::delscreen(sc);
	nc::endwin();
	return 0;
}
