#pragma once

#include <robot_link.h>

/**
	Wraps robot_link to indicate failures by throwing a LinkError object
*/
class RLink : public robot_link {
public:
	/// Initialise the link by the most appropriate method for the location the code is running
	void initialise();

	/// Send a command to the robot
	void command(command_instruction cmd, int arg);

	/// Request data from the robot
	int request(request_instruction req);

	/**
		Get the status register, a bitfield containing
		{comm_err, i2c_err, es_trig, es_mode,
		moving, ramped, _, _}

		Does not throw LinkError
	*/
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
	/// @brief The original error code
	const link_err err;

	/// Whether the error is marked as fatal by robot_link
	const bool is_fatal;

	/// If true, indicates that the error has no code, and is instead a bus error
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

	/// override of std::exception::what()
	virtual const char* what() const throw() {
		if(err != LINKERR_NONE) return _r.err_string(err);
		if(is_i2c) return "I2C bus error - board unplugged?";
		return "No error code available";
	}
};
