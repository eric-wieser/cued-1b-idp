#include "../dev/ports.h"
#include "../dev/rlink.h"
#include <cstdint>
#include <iostream>
#include <chrono>
#include "utils/timeout.h"

typedef std::chrono::steady_clock clock;

int main() {
	RLink r;
	std::cout << "constructed" << std::endl;
	{
		clock::time_point start_t = clock::now();
		r.initialise();
		std::chrono::nanoseconds dt = clock::now() - start_t;
		cout << "Initialize time: " << dt.count() << "ns" << std::endl;
	}

	int N = 10000;
	{
		clock::time_point start_t = clock::now();
		for(int i = 0; i < N; i++) {
			r.command(WRITE_PORT_0, i%256);
		}
		std::chrono::nanoseconds dt = clock::now() - start_t;
		cout << "Average command time: " << (dt / N).count() << "ns" << std::endl;
	}
	{
		clock::time_point start_t = clock::now();
		for(int i = 0; i < N; i++) {
			r.command(READ_PORT_0);
		}
		std::chrono::nanoseconds dt = clock::now() - start_t;
		cout << "Average request time: " << (dt / N).count() << "ns" << std::endl;
	}
}