#pragma once

#include <robot_link.h>

/**
	Wraps robot_link to indicate failures with exceptions

	.status() is special, as error checking should not itself throw an exception
*/
class RLink : public robot_link {
public:
	void initialise();
	void command(command_instruction cmd, int arg);
	int request(request_instruction req);
	uint8_t status();
};

/**
	Thrown when an RLink command or request goes wrong. Contains the original
	error code
*/
class LinkError : public std::exception {
private:
	RLink& _r;
public:
	const link_err err;
	const bool is_fatal;
	const bool is_i2c;

	LinkError(RLink& r) :
		_r(r),
		err(r.lookup_err(0)),
		is_fatal(r.fatal_err()),
		is_i2c(r.status() & 0x2) {}

	virtual ~LinkError() throw() { _r.clear_errs(); }
	inline void log() {
		_r.print_errs("\t");
	}

	/// override of std::exception
	virtual const char* what() const throw() {
		if(err != LINKERR_NONE) return _r.err_string(err);
		if(is_i2c) return "I2C bus error - board unplugged?";
		return "No error code available";
	}
};
