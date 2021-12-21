#include "internal.h"

#ifdef __unix__

const CL_GlobalInfo g_cl_info =
{
	.color = {
		"\e[38;5;54m",
		"\e[38;5;160m",
		"\e[38;5;226m",
		"\e[38;5;40m",
		"\e[38;5;248m",
		"\e[0m"},
	.log_level_names = { "FATAL", "ERROR", "WARN", "INFO", "TRACE"}
};

#elif defined _WIN32 || defined WIN32

const CL_GlobalInfo g_cl_info =
{
	.color = {
		WINDOWS_MAGENTA,
		WINDOWS_RED,
		WINDOWS_YELLOW,
		WINDOWS_GREEN,
		WINDOWS_LIGHTGRAY,
		WINDOWS_LIGHTGRAY
	},
	.log_level_names = { "FATAL", "ERROR", "WARN", "INFO", "TRACE"}
};

void textcolor(int color)
{
	static int __BACKGROUND;

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;


	GetConsoleScreenBufferInfo(h, &csbiInfo);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		color + (__BACKGROUND << 4));
}

#endif

void _cl_logger_log(CL_Logger *logger, uint32_t lvl, const char *file, uint32_t line, const char *format, ...)
{
	if (logger->log_level >= lvl)
	{
		va_list va_args;
		va_start(va_args, format);
		CL_TimeInfo time;
		_CL_LOG_UPDATE_TIME(time);
		for (uint32_t i = 0; i < logger->output_count_c; i++)
		{
			uint32_t color_set = 0;
			for (uint32_t segment_index = 0; segment_index < logger->pattern.segment_count_c; segment_index++)
			{
				switch (logger->pattern.segment_types[segment_index])
				{
				case CL_SEGMENT_TYPE_FILE:
					fprintf(logger->outputs[i], "%s", file);
					break;
				case CL_SEGMENT_TYPE_LINE:
					fprintf(logger->outputs[i], "%d", line);
					break;
				case CL_SEGMENT_TYPE_LOG_LEVEL:
					fprintf(logger->outputs[i], "%s", g_cl_info.log_level_names[lvl]);
					break;
				case CL_SEGMENT_TYPE_TIME:
					fprintf(logger->outputs[i], "%s", time.string);
					break;
				case CL_SEGMENT_TYPE_MESSAGE:
					vfprintf(logger->outputs[i], format, va_args);
					break;
				case CL_SEGMENT_TYPE_NAME:
					fprintf(logger->outputs[i], "%s", logger->name);
					break;
				case CL_SEGMENT_TYPE_COLOR:
					if (logger->outputs[i] == stdout || logger->outputs[i] == stderr)
					{
						if (color_set)
						{
							_CL_LOG_COLOR_RESET();
							color_set = 0;
						}
						else
						{
							_CL_LOG_COLOR_SET(lvl);
							color_set = 1;
						}
					}
					break;
				case CL_SEGMENT_TYPE_STRING:
					fprintf(logger->outputs[i], "%s", logger->pattern.segment_values[segment_index]);
					break;
				}
			}
			fprintf(logger->outputs[i], "\n");
		}
		va_end(va_args);
	}
}

CL_Logger *_cl_logger_create(uint16_t ouput_count, const char *name, const char *pattern)
{
	CL_Logger *logger = (CL_Logger *)malloc(sizeof(*logger));
	logger->log_level = CL_LOG_LEVEL_TRACE;
	logger->output_count_c = 0;
	if (ouput_count == 0)
		ouput_count = 2;
	logger->output_count_m = ouput_count;
	logger->outputs = malloc(sizeof(*logger->outputs) * ouput_count);
	if (name == NULL)
		name = "UNNAMED";
	logger->name = name;

	if (pattern == NULL)
	{
		pattern = CL_DEFAULT_PATTERN;
	}

	logger->pattern.string = pattern;
	logger->pattern.segment_count_c = 0;
	logger->pattern.segment_count_m = 16;
	logger->pattern.segment_types = malloc(sizeof(*logger->pattern.segment_types) * logger->pattern.segment_count_m);
	logger->pattern.segment_values = malloc(sizeof(*logger->pattern.segment_values) * logger->pattern.segment_count_m);

	// compile pattern
	{
		uint64_t pattern_length = strlen(pattern);
		const char *segment_start = pattern;
		for (uint32_t pattern_index = 0; pattern_index < pattern_length; pattern_index++)
		{
			if (pattern[pattern_index] == '%')
			{
				if (logger->pattern.segment_count_c + 2 >= logger->pattern.segment_count_m)
				{
					logger->pattern.segment_count_m *= 2;
					void *types_ptr = realloc(logger->pattern.segment_types, sizeof(*logger->pattern.segment_types) * logger->pattern.segment_count_m);
					void *values_ptr = realloc(logger->pattern.segment_values, sizeof(*logger->pattern.segment_values) * logger->pattern.segment_count_m);

					if (types_ptr == NULL || values_ptr == NULL)
					{
						fprintf(stderr, "Couldn't realloc memory for pattern segments.\n");
					}
					else
					{
						logger->pattern.segment_types = types_ptr;
						logger->pattern.segment_values = values_ptr;
					}

				}
				// push string
				{
					const char *segment_end = pattern + pattern_index;
					uint64_t segment_length = segment_end - segment_start;
					if (segment_length != 0)
					{
						char *string = malloc(sizeof(*string) * (segment_length + 1));
						for (uint32_t string_index = 0; string_index < segment_length; string_index++)
						{
							string[string_index] = segment_start[string_index];
						}
						string[segment_length] = '\0';
						logger->pattern.segment_types[logger->pattern.segment_count_c] = CL_SEGMENT_TYPE_STRING;
						logger->pattern.segment_values[logger->pattern.segment_count_c] = string;
						logger->pattern.segment_count_c++;
					}
				}

				pattern_index++;
				switch (pattern[pattern_index])
				{
				case 'F':
					logger->pattern.segment_types[logger->pattern.segment_count_c] = CL_SEGMENT_TYPE_FILE;
					break;
				case 'L':
					logger->pattern.segment_types[logger->pattern.segment_count_c] = CL_SEGMENT_TYPE_LINE;
					break;
				case 'V':
					logger->pattern.segment_types[logger->pattern.segment_count_c] = CL_SEGMENT_TYPE_LOG_LEVEL;
					break;
				case 'T':
					logger->pattern.segment_types[logger->pattern.segment_count_c] = CL_SEGMENT_TYPE_TIME;
					break;
				case 'M':
					logger->pattern.segment_types[logger->pattern.segment_count_c] = CL_SEGMENT_TYPE_MESSAGE;
					break;
				case 'N':
					logger->pattern.segment_types[logger->pattern.segment_count_c] = CL_SEGMENT_TYPE_NAME;
					break;
				case 'C':
					logger->pattern.segment_types[logger->pattern.segment_count_c] = CL_SEGMENT_TYPE_COLOR;
					break;
				}
				if (pattern[pattern_index] == '%')
				{
					char *string = malloc(sizeof(*string) * 2);
					string[0] = '%';
					string[1] = '\0';
					logger->pattern.segment_types[logger->pattern.segment_count_c] = CL_SEGMENT_TYPE_STRING;
					logger->pattern.segment_values[logger->pattern.segment_count_c] = string;
				}
				else
				{
					logger->pattern.segment_values[logger->pattern.segment_count_c] = NULL;
				}
				logger->pattern.segment_count_c++;

				segment_start = pattern + pattern_index + 1;
			}
		}
		const char *segment_end = pattern + pattern_length + 1;
		uint64_t segment_length = segment_end - segment_start;
		if (segment_length != 0)
		{
			if (logger->pattern.segment_count_c + 1 >= logger->pattern.segment_count_m)
			{
				logger->pattern.segment_count_m *= 2;
				void *types_ptr = realloc(logger->pattern.segment_types, sizeof(*logger->pattern.segment_types) * logger->pattern.segment_count_m);
				void *values_ptr = realloc(logger->pattern.segment_values, sizeof(*logger->pattern.segment_values) * logger->pattern.segment_count_m);

				if (logger->pattern.segment_types == NULL || logger->pattern.segment_values)
				{
					fprintf(stderr, "Couldn't realloc memory for pattern segments.\n");
				}
				else
				{
					logger->pattern.segment_types = types_ptr;
					logger->pattern.segment_values = values_ptr;
				}
			}
			char *string = malloc(sizeof(*string) * (segment_length + 1));
			for (uint32_t string_index = 0; string_index < segment_length; string_index++)
			{
				string[string_index] = segment_start[string_index];
			}
			string[segment_length] = '\0';
			logger->pattern.segment_types[logger->pattern.segment_count_c] = CL_SEGMENT_TYPE_STRING;
			logger->pattern.segment_values[logger->pattern.segment_count_c] = string;
			logger->pattern.segment_count_c++;
		}
	}

	CL_LOG_INFO(logger, "Logger %s initialized succesfully.", name);

	return logger;
}

void _cl_logger_output_add(CL_Logger *logger, FILE *file)
{
	if (logger->output_count_c == logger->output_count_m)
	{
		logger->output_count_m *= 2;
		logger->outputs = realloc(logger->outputs, sizeof(*logger->outputs) * logger->output_count_m);
	}
	logger->outputs[logger->output_count_c] = file;
	logger->output_count_c++;
}

void _cl_logger_lvl_set(CL_Logger *logger, CL_LogLevel lvl)
{
	logger->log_level = lvl;
}

CL_LogLevel _cl_logger_lvl_get(CL_Logger *logger)
{
	return logger->log_level;
}

void _cl_logger_destroy(CL_Logger *logger)
{
	for (uint32_t i = 0; i < logger->pattern.segment_count_c; i++)
	{
		if (logger->pattern.segment_values[i] != NULL)
			free(logger->pattern.segment_values[i]);
	}
	free(logger->pattern.segment_values);
	free(logger->pattern.segment_types);
	free(logger->outputs);
	free(logger);
}

void _log_level_set(CL_Logger *logger, uint32_t lvl)
{
	if (lvl < CL_LOG_LEVEL_COUNT)
		logger->log_level = lvl;
	else
	{
		//LOG_ERROR("Invalid log level: %d. Please make sure you use the enum provided to set a log level.", lvl);
	}
}