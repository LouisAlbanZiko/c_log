#ifndef _CL_LOG_H_
#define _CL_LOG_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

typedef struct _CL_Logger CL_Logger;

#ifndef CL_DEFAULT_PATTERN
#define CL_DEFAULT_PATTERN "[%T] (%N) %C%V\t%F, %L %M%C"
#endif

typedef enum CL_LogLevel
{
	CL_LOG_LEVEL_FATAL = 0,
	CL_LOG_LEVEL_ERROR,
	CL_LOG_LEVEL_WARN,
	CL_LOG_LEVEL_INFO,
	CL_LOG_LEVEL_TRACE,
	CL_LOG_LEVEL_COUNT
} CL_LogLevel;

#ifdef CL_LOG_DISABLED

#define CL_LOG_MESSAGE(logger, level, ...)

#define CL_LOG(logger, level, ...)

#define CL_LOG_TRACE(logger, ...)
#define CL_LOG_INFO(logger, ...)
#define CL_LOG_WARN(logger, ...)
#define CL_LOG_ERROR(logger, ...)
#define CL_LOG_FATAL(logger, ...)

#define CL_LOGGER_CREATE(name, pattern, ...) NULL
#define CL_LOGGER_LVL_SET(logger, lvl)
#define CL_LOGGER_LVL_GET(logger)
#define CL_LOGGER_DESTROY(logger)

#else

#define CL_LOG(logger, level, ...) _cl_logger_log(logger, level, __FILE__, __LINE__, __VA_ARGS__)

#define CL_LOG_TRACE(logger, ...) CL_LOG(logger, CL_LOG_LEVEL_TRACE, __VA_ARGS__)
#define CL_LOG_INFO(logger, ...) CL_LOG(logger, CL_LOG_LEVEL_INFO, __VA_ARGS__)
#define CL_LOG_WARN(logger, ...) CL_LOG(logger, CL_LOG_LEVEL_WARN, __VA_ARGS__)
#define CL_LOG_ERROR(logger, ...) CL_LOG(logger, CL_LOG_LEVEL_ERROR, __VA_ARGS__)
#define CL_LOG_FATAL(logger, ...) { CL_LOG(logger, CL_LOG_LEVEL_FATAL, __VA_ARGS__); exit(EXIT_FAILURE); }

#define CL_LOGGER_CREATE(name, pattern, ...) _cl_logger_create(name, pattern, __VA_ARGS__)
#define CL_LOGGER_LVL_SET(logger, lvl) _cl_logger_lvl_set(logger, lvl)
#define CL_LOGGER_LVL_GET(logger) _cl_logger_lvl_get(logger)
#define CL_LOGGER_DESTROY(logger) _cl_logger_destroy(logger)

#endif

CL_Logger *_cl_logger_create(const char *name, const char *pattern, uint64_t count, ...);
void _cl_logger_output_add(CL_Logger *logger, FILE *file);
void _cl_logger_lvl_set(CL_Logger *logger, CL_LogLevel lvl);
CL_LogLevel _cl_logger_lvl_get(CL_Logger *logger);
void _cl_logger_destroy(CL_Logger *logger);


#endif