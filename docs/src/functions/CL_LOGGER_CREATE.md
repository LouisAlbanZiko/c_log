# CL_LOGGER_CREATE

```c
CL_Logger *CL_LOGGER_CREATE(const char *name, const char *pattern, uint64_t output_count, ...);
```

#### Parameters

- `name` -> the name of the logger which can be output when logging if it is contained in the pattern. If left NULL the logger will be named "UNNAMED".
- `pattern` -> a string that will define the format of the output when logging. It can contain any character and any of the following formats:
	- `%F` -> the source file at which the logging was done (\_\_FILE\_\_ macro)
	- `%L` -> the line of the file at which the logging was done (\_\_LINE\_\_ macro)
	- `%T` -> the time at which the logging was done in HH:MM:SS format
	- `%M` -> the message that is passed to the loggin function. If %M is not present in the pattern the message will not be printed.
	- `%N` -> the name of the logger
	- `%C` -> the start and end of color. ex: %C Hello world %C ; in this case Hello world will be printed in color.
	- `%V` -> the name of the log level which can be TRACE, INFO, WARN, ERROR or FATAL

	If NULL is passed the pattern will be set to CL_DEFAULT_PATTERN which is defined in `c_log.h`.
	To insert a `%` character in the pattern write `%%`.
- `output_count` -> the count of how many `FILE *` are to be passed as the next parameters.
- `...` -> the `FILE *`(s) to where the logger will output.

The function returns the pointer to the logger.

#### Usage
To output with default pattern:
```c
CL_Logger *logger = CL_LOGGER_CREATE("TEST", NULL, 1, stdout);
```
To output with custom pattern:
```c
CL_Logger *logger = CL_LOGGER_CREATE("TEST", "%C[%V]%C(%F,%L)%C%M%C", 1, stdout);
```
To output to stderr:
```c
CL_Logger *logger = CL_LOGGER_CREATE("TEST", NULL, 1, stderr);
```
To output to a file:
```c
FILE *file = fopen("log.txt", "w");
CL_Logger *logger = CL_LOGGER_CREATE("TEST", NULL, 1, file);
```
To write to multiple outputs:
```c
FILE *file = fopen("log.txt", "w");
CL_Logger *logger = CL_LOGGER_CREATE("TEST", NULL, 2, stderr file);
```