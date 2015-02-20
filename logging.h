enum LogLevel {
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_WARN,
	LEVEL_ERROR
};

#if !defined(LOGGING)
	#define LOGGING LEVEL_WARN
#end

#define DEBUG(x) (LOGGING <= LEVEL_DEBUG && (x));
#define INFO(x)  (LOGGING <= LEVEL_INFO && (x));
#define WARN(x)  (LOGGING <= LEVEL_WARN && (x));
#define ERROR(x) (LOGGING <= LEVEL_ERROR && (x));

