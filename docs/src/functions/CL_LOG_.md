# CL_LOG_...

```c
void CL_LOG_TRACE(Logger *logger, const char *format, ...);
void CL_LOG_INFO(Logger *logger, const char *format, ...);
void CL_LOG_WARN(Logger *logger, const char *format, ...);
void CL_LOG_ERROR(Logger *logger, const char *format, ...);
void CL_LOG_FATAL(Logger *logger, const char *format, ...);
```

These functions are used to log using a specific logger.
A new line is always printed at the end of the function.

#### Parameters

- `logger` -> The logger used to print.
- `format` and `...` -> The message to log. These two parameters work the same as for `printf` in the standard library.

#### Usage

To print a random integer as INFO:
```c
CL_Logger *logger = CL_LOGGER_CREATE("TEST", NULL, 1, stdout);
// ...
CL_LOG_INFO(logger, "%d", rand());
// ...
CL_LOGGER_DESTROY(logger);
```