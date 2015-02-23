
#include <cstdint>
#include <iostream>
#include <chrono>
#include "../util/timeout.h"
#include "../dev/ports.h"
#include "../dev/rlink.h"

using namespace std::chrono;

typedef system_clock clk;

int main() {
	RLink r;
	std::cout << "constructed" << std::endl;
	{
		clk::time_point start_t = clk::now();
		r.initialise();
		microseconds dt = duration_cast<microseconds>(clk::now() - start_t);
		std::cout << "Initialize time: " << dt.count() << "us" << std::endl;
	}

	int N = 10000;
	{
		clk::time_point start_t = clk::now();
		for(int i = 0; i < N; i++) {
			r.command(WRITE_PORT_0, i%256);
		}
		microseconds dt = duration_cast<microseconds>(clk::now() - start_t);
		std::cout << "Average command time: " << (dt / N).count() << "us" << std::endl;
	}
	{
		clk::time_point start_t = clk::now();
		for(int i = 0; i < N; i++) {
			r.request(READ_PORT_0);
		}
		microseconds dt = duration_cast<microseconds>(clk::now() - start_t);
		std::cout << "Average request time: " << (dt / N).count() << "us" << std::endl;
	}
}