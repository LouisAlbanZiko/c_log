#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <c_log.h>


int main(int argc, char **argv)
{
	CL_INIT();

	CL_Logger *logger = CL_LOGGER_CREATE(1, "DEFAULT");
	CL_LOGGER_OUTPUT_ADD(logger, stdout, CL_COLOR_ENABLED);

	CL_Logger *logger2 = CL_LOGGER_CREATE(1, "LOGGER2");
	CL_LOGGER_OUTPUT_ADD(logger2, stdout, CL_COLOR_DISABLED);

	CL_LOG_TRACE(logger, "Hello World! %d", rand());
	CL_LOG_INFO(logger, "Hello World! %d", rand());
	CL_LOG_WARN(logger, "Hello World! %d", rand());
	CL_LOG_ERROR(logger, "Hello World! %d", rand());
	CL_LOG_FATAL(logger, "Hello World! %d", rand());

	CL_LOG_TRACE(logger2, "Hello World! %d", rand());
	CL_LOG_INFO(logger2, "Hello World! %d", rand());
	CL_LOG_WARN(logger2, "Hello World! %d", rand());
	CL_LOG_ERROR(logger2, "Hello World! %d", rand());
	CL_LOG_FATAL(logger2, "Hello World! %d", rand());

	CL_LOGGER_DESTROY(logger);

	CL_TERMINATE();
}