# CL_LOG_LVL_SET

```c
void CL_LOGGER_LVL_SET(CL_Logger *logger, CL_LogLevel lvl);
```

Sets the [log level](../ch3_log_levels.md) at which the logger is to output.

#### Parameters

- `logger` -> the logger to modify
- `lvl` -> the [log level](../ch3_log_levels.md) to set

#### Usage

To set the level to INFO:
```c
CL_Logger *logger = CL_LOGGER_CREATE("TEST", NULL, 1, stdout);
CL_LOGGER_LVL_SET(logger, CL_LOG_LEVEL_INFO);
// ...
CL_LOG_TRACE(logger, "will not be output")
CL_LOG_INFO(logger, "will be output");
CL_LOG_WARN(logger, "will be output");
// ...
CL_LOGGER_DESTROY(logger);
```