# CL_LOG_LVL_GET

```c
CL_LogLevel CL_LOGGER_LVL_GET(CL_Logger *logger);
```

Returns the [log level](../ch3_log_levels.md) of the passed logger.

#### Parameters

- `logger` -> the logger from which the [log level](../ch3_log_levels.md) is retrieved.

### Usage
```c
CL_Logger *logger = CL_LOGGER_CREATE(&quot;TEST&quot;, NULL, 1, stdout);
// ...
CL_LogLevel level = CL_LOGGER_LVL_GET(logger);
// ...
CL_LOGGER_DESTROY(logger);
```