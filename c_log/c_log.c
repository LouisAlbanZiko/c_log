#include "internal.h"

CL_GlobalInfo *g_cl_info = NULL;

void _cl_logger_log(CL_Logger *logger, uint32_t lvl, const char *file, uint32_t line, const char *format, ...)
{
	if (logger->log_level >= lvl)
	{
		va_list va_args;
		va_start(va_args, format);
		_CL_LOG_UPDATE_TIME();
		for (uint32_t i = 0; i < logger->output_count_c; i++)
		{
			if(logger->output_colors[i])
			{
				_CL_LOG_COLOR_SET(lvl);
			}
			_cl_vflog(logger->outputs[i], lvl, file, line, format, va_args);
			if(logger->output_colors[i])
			{
				_CL_LOG_COLOR_RESET();
			}
		}
		va_end(va_args);
	}
}

/*void _cl_vlog(CL_Logger *logger, uint32_t lvl, const char *file, uint32_t line, const char *format, va_list va_args)
{
	_cl_vflog(stdout, lvl, file, line, format, va_args);
	if (logger->file_output != NULL)
	{
		_cl_vflog(logger->file_output, lvl, file, line, format, va_args);
	}
}

void _cl_flog(CL_Logger *logger, FILE *output, uint32_t lvl, const char *file, uint32_t line, const char *format, ...)
{
	va_list va_args;
	va_start(va_args, format);
	_cl_vflog(logger, output, )
	va_end(va_args);
}*/

void _cl_vflog(FILE *output, uint32_t lvl, const char *file, uint32_t line, const char *format, va_list va_args)
{
	fprintf(output, "[%s] ", g_cl_info->time.string);
	fprintf(output, "%s", g_cl_info->log_level_names[lvl]);
	fprintf(output, " \t%s:%d\t", file, line);
	vfprintf(output, format, va_args);
	fprintf(output, "\n");
}

CL_Logger *_cl_logger_create(uint16_t ouput_count, const char *name)
{
	if (g_cl_info == NULL)
		_cl_init();

	CL_Logger *logger = (CL_Logger *)malloc(sizeof(*logger));
	logger->log_level = CL_LOG_LEVEL_TRACE;
	logger->output_count_c = 0;
	logger->output_count_m = ouput_count;
	logger->outputs = malloc(sizeof(*logger->outputs) * ouput_count);
	logger->output_colors = malloc(sizeof(*logger->output_colors) * ouput_count);
	logger->name = name;

	//LOG_INFO("Logger %s initialized succesfully.", name);

	return logger;
}

void _cl_logger_output_add(CL_Logger *logger, FILE *file, uint8_t color_output)
{
	logger->outputs[logger->output_count_c] = file;
	logger->output_colors[logger->output_count_c] = color_output;
	logger->output_count_c++;
}

void _cl_logger_destroy(CL_Logger *logger)
{
	free(logger->outputs);
	free(logger->output_colors);
	free(logger);
}

void _cl_init()
{
	g_cl_info = malloc(sizeof(*g_cl_info));
	g_cl_info->color[CL_LOG_LEVEL_TRACE] = "\e[38;5;248m";
	g_cl_info->color[CL_LOG_LEVEL_INFO] = "\e[38;5;40m";
	g_cl_info->color[CL_LOG_LEVEL_WARN] = "\e[38;5;226m";
	g_cl_info->color[CL_LOG_LEVEL_ERROR] = "\e[38;5;160m";
	g_cl_info->color[CL_LOG_LEVEL_FATAL] = "\e[38;5;54m";
	g_cl_info->color[CL_LOG_LEVEL_COUNT] = "\e[0m";
	g_cl_info->log_level_names[CL_LOG_LEVEL_TRACE] = "TRACE";
	g_cl_info->log_level_names[CL_LOG_LEVEL_INFO] = "INFO";
	g_cl_info->log_level_names[CL_LOG_LEVEL_WARN] = "WARN";
	g_cl_info->log_level_names[CL_LOG_LEVEL_ERROR] = "ERROR";
	g_cl_info->log_level_names[CL_LOG_LEVEL_FATAL] = "FATAL";
}

void _cl_terminate()
{
	if (g_cl_info != NULL)
		free(g_cl_info);
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