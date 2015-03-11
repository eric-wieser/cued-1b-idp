#pragma once
#include <cstdint>
#include <robot_instr.h>
#include <sstream>
#include <string>
#include "rlink.h"
#include "device.h"

namespace port {
	/// List of all the port names we could use
	enum Name{
		P0,  P1,  P2,  P3,  P4,  P5,  P6,  P7,
		PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
		NUM_PORTS
	};

	/// get the write instruction corresponding to a given port
	inline command_instruction write_instr(Name p) {
		if(p <= P7)
			return static_cast<command_instruction>(WRITE_PORT_0 + p);
		else
			return static_cast<command_instruction>(WRITE_PORT_A0 + p);
	}

	/// get the read instruction corresponding to a given port
	inline request_instruction read_instr(Name p) {
		if(p <= P7)
			return static_cast<request_instruction>(READ_PORT_0 + p);
		else
			return static_cast<request_instruction>(READ_PORT_A0 + p);
	}
}

inline std::ostream& operator <<(std::ostream& stream, const port::Name& p) {
	if(port::P0 <= p && p < port::PA0)
		return stream << "P" << int(p);
	else if(port::PA0 <= p && p < port::NUM_PORTS)
		return stream << "PA" << int(p);
	else
		return stream << "P?" << int(p);
}

/**
	Specialization of LinkError, thrown when an I2C error occurs
	when accessing a port. Typically implies loss of electrical
	connection
*/
struct PortError : public LinkError {
public:
	/// the disconnected port
	const port::Name port;

	PortError(const LinkError& le, port::Name p)
			: LinkError(le), port(p) {}

	virtual const char* what() const throw() {
		switch (port) {
			case port::P0: return "Port P0 disconnected.";
			case port::P1: return "Port P1 disconnected.";
			case port::P2: return "Port P2 disconnected.";
			case port::P3: return "Port P3 disconnected.";
			case port::P4: return "Port P4 disconnected.";
			case port::P5: return "Port P5 disconnected.";
			case port::P6: return "Port P6 disconnected.";
			case port::P7: return "Port P7 disconnected.";
			default:       return "Other port disconnected";
		}
	}
};


/**
	Specialization of LinkError, thrown when an I2C error occurs
	when accessing a port. Typically implies loss of electrical
	connection
*/
struct PinsDoublyMapped : public std::exception {
private:
	mutable std::string _message;
public:
	const port::Name port;  ///< the port causing the issue
	const uint8_t pins;     ///< the mask of pins that have already been allocated

	PinsDoublyMapped(port::Name p, uint8_t mask): port(p), pins(mask), _message() {}
	~PinsDoublyMapped() throw() {}

	virtual const char* what() const throw() {
		if(_message == "") {
			std::ostringstream ss;
			ss << "Attempted to double map port " << port << ", pins:";
			for(int i = 0; i < 8; i++) if((pins >> i) & 1) ss << " " << i;
			_message = ss.str();
		}

		return _message.c_str();
	}
};

/**
	Interface to a set on pins on a particular port.
	Allows masking of pins, to allow multiple `Device`s to share a I2C port
	without interfering with each other's bits

	Provides operator overloading for simple use:

		Port sensor(rlink, port::P2, 0xF);  // bottom 4 bits of port 2
		uint8_t reading = sensor;  // read sensor
		sensor = 0x42;  // write to sensor

	Note that conversion to an int will return the current input, which is not
	necesarily the previous output
*/
class Port : public Device {
private:
	static uint8_t lastWrites[port::NUM_PORTS];
	static uint8_t assignedBits[port::NUM_PORTS];
	port::Name _port;
	uint8_t _mask;
public:
	/**
		Create a port over the connection `r`, using the port with address `p`.
		Optionally specify a set of bits `mask` to restrict the scope of this instance
		to. Throws PinsDoublyMapped if multiple instances attempt to use the same ports
	*/
	Port(RLink& r, port::Name p, uint8_t mask=0xFF) : Device(r), _port(p), _mask(mask) {
		if(mask & assignedBits[p]) {
			throw PinsDoublyMapped(p, mask & assignedBits[p]);
		}
		assignedBits[p] = mask;
	}
	~Port() {
		// unassign this port
		assignedBits[_port] &= ~_mask;
	}

	/**
		Read a word to the port, keeping only the bits specified in the mask
	*/
	inline operator uint8_t() const {
		try {
			uint8_t word = _r.request(port::read_instr(_port));
			return word & _mask;
		}
		catch (const LinkError& le) {
			if (!le.is_i2c) {
				throw;
			}

			throw PortError(le, _port);
		}
	}

	/**
		Write a word to the port, touching only the bits specified in the mask
	*/
	inline void operator=(uint8_t val) {
		if(_mask != 0xFF) {
			// only bother reading if some bits are not masked
			val = (val & _mask) | (~_mask & lastWrites[_port]);
		}
		lastWrites[_port] = val;
		try {
			_r.command(port::write_instr(_port), val);
		}
		catch (const LinkError& le) {
			if (!le.is_i2c) {
				throw;
			}

			throw PortError(le, _port);
		}
	}

	inline Port& operator|=(uint8_t val) { *this = lastWrites[_port] | val; return *this; }
	inline Port& operator&=(uint8_t val) { *this = lastWrites[_port] & val; return *this; }
};
