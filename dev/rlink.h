#pragma once

#include <robot_link.h>

class LinkError;

/**
	Wraps robot_link to indicate failures with exceptions
 */
class RLink : public robot_link {
public:
	void initialise();
	void command(command_instruction cmd, int arg);
	int request(request_instruction req);
};

class LinkError {
private:
	RLink& _r;
public:
	const bool is_fatal;
	LinkError(RLink& r) :_r(r), is_fatal(r.fatal_err()) {};
	~LinkError() { _r.clear_errs(); };
	inline void log() {
		_r.print_errs("\t");
	}
};
