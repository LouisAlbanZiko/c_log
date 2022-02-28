# CL_LOGGER_DESTROY

```c
void CL_LOGGER_DESTROY(Logger *logger);
```
This function will free the data related to the logger.
The passed logger cannot be used after calling this function.


#### Usage

```c
CL_Logger *logger = CL_LOGGER_CREATE(&quot;TEST&quot;, NULL, 1, stdout);

// ...

CL_LOGGER_DESTROY(logger);
```