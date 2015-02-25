

#define LEVEL_DEBUG 1
#define LEVEL_INFO 2
#define LEVEL_WARN 3
#define LEVEL_ERROR 4

#ifndef LOGGING
	#define LOGGING LEVEL_WARN
#endif

#if LOGGING <= LEVEL_DEBUG
	#define DEBUG(x) (x)
#else
	#define DEBUG(x) 
#endif

#if LOGGING <= LEVEL_INFO
	#define INFO(x) (x)
#else
	#define INFO(x) 
#endif

#if LOGGING <= LEVEL_WARN
	#define WARN(x) (x)
#else
	#define WARN(x) 
#endif

#if LOGGING <= LEVEL_ERROR
	#define ERROR(x) (x)
#else
	#define ERROR(x) 
#endif
