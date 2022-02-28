# Log Levels

The different levels a logger can be at:

```c
typedef enum CL_LogLevel
{
	CL_LOG_LEVEL_FATAL = 0,
	CL_LOG_LEVEL_ERROR,
	CL_LOG_LEVEL_WARN,
	CL_LOG_LEVEL_INFO,
	CL_LOG_LEVEL_TRACE
} CL_LogLevel;
```

#### Usage

No output will occur if the level of the logger is higher than the level of the function being called.
For example:
```c
CL_Logger *logger = CL_LOGGER_CREATE("TEST", NULL, 1, stdout);
CL_LOGGER_LVL_SET(logger, CL_LOG_LEVEL_WARN);
// ...
CL_LOG_INFO(logger, "will not be output");
CL_LOG_WARN(logger, "will be output");
CL_LOG_ERROR(logger, "will be output");
// ...
CL_LOGGER_DESTROY(logger);
```

By default the level of a logger is `CL_LOG_LEVEL_TRACE`.