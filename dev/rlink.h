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

class LinkError : public std::exception {
private:
	RLink& _r;
public:
	const link_err err;
	const bool is_fatal;

	LinkError(RLink& r) : _r(r), err(r.lookup_err(0)), is_fatal(r.fatal_err()) {}
	virtual ~LinkError() throw() { _r.clear_errs(); }
	inline void log() {
		_r.print_errs("\t");
	}

	/// override of std::exception
	virtual const char* what() const throw() {
		return err == LINKERR_NONE ? "Undescribed error" : _r.err_string(err);
	}
};
