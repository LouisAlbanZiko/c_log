## Setup

#### With Premake
Clone the repository to the location of your choice. Create a new premake workspace and include the file c_log/premake5.lua inside your own premake file. Then link your executable project to the c_log project using this line links `c_log`. Finally compile the project. For a full setup of premake go to the premake [github page](https://github.com/premake/premake-core).

#### Without Premake
Copy the files c_log.c, c_log.h and internal.h into your project directory and compile your project with these files.

## Including to project
Once added to your project the file can be included as follows. Make sure the c_log.h file is in the include path of your project.

```c
#include <c_log.h>
```

## Example
The following example will create 2 loggers and show some output to demonstrate the capabilities of the library.

```c
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

//#define CL_LOG_DISABLED
#include <c_log.h>

int main(int argc, char **argv)
{
	CL_Logger *logger = CL_LOGGER_CREATE("LOGGER", "[%T] (%N) %% %C%V\t%F, %L %M%C", 1, stdout);

	CL_Logger *logger2 = CL_LOGGER_CREATE("LOGGER2", NULL, 1, stdout);

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
	CL_LOGGER_DESTROY(logger2);
}
```