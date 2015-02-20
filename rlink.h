#include <robot_link.h>

class LinkError {
private:
	RLink& _r;
public:
	const bool is_fatal;
	LinkError(RLink& r) :_r(r), is_fatal(r.fatal_err()) {};
	~LinkError() { r.clear_errs(); };
	inline void log() {
		r.print_errs("\t");
	}
};

/**
	Wraps robot_link to indicate failures with exceptions
 */
class RLink : public robot_link {
public:
	inline void initialise() {
		#ifdef __arm__
			bool ok = robot_link::initialise();
		#else
			bool ok = robot_link::initialise(5); // team no
		#endif

		if(!ok) throw LinkError(*this);
	}
	inline void command(command_instruction cmd, int arg) {
		bool ok = robot_link::command(cmd, arg);
		if(!ok) throw LinkError(*this);
	}
	inline int request(request_instruction req) {
		int res = robot_link::request(req);
		if(res == REQUEST_ERROR) throw LinkError(*this);
		return res;
	}
};
