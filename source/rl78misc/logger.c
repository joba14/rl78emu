
/**
 * @file logger.c
 * 
 * @copyright This file is a part of the "rl78emu" project and is licensed, and
 * distributed under "rl78emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-03
 */

#include "rl78misc/debug.h"
#include "rl78misc/logger.h"

#include <stdarg.h>
#include <stdio.h>

static void log_with_tag(
	FILE* const stream,
	const char_t* const tag,
	const char_t* const format,
	va_list args);

void rl78misc_logger_log(
	const char_t* const format,
	...)
{
	rl78misc_debug_assert(format != NULL);
	va_list args; va_start(args, format);
	log_with_tag(stdout, NULL, format, args);
	va_end(args);
}

void rl78misc_logger_info(
	const char_t* const format,
	...)
{
	rl78misc_debug_assert(format != NULL);
	va_list args; va_start(args, format);
	log_with_tag(stdout, "\033[92m" "info" "\033[0m", format, args);
	va_end(args);
}

void rl78misc_logger_warn(
	const char_t* const format,
	...)
{
	rl78misc_debug_assert(format != NULL);
	va_list args; va_start(args, format);
	log_with_tag(stderr, "\033[93m" "warn" "\033[0m", format, args);
	va_end(args);
}

void rl78misc_logger_error(
	const char_t* const format,
	...)
{
	rl78misc_debug_assert(format != NULL);
	va_list args; va_start(args, format);
	log_with_tag(stderr, "\033[91m" "error" "\033[0m", format, args);
	va_end(args);
}

static void log_with_tag(
	FILE* const stream,
	const char_t* const tag,
	const char_t* const format,
	va_list args)
{
	rl78misc_debug_assert(stream != NULL);
	rl78misc_debug_assert(format != NULL);

	if (tag != NULL)
	{
		(void)fprintf(stream, "%s: ", tag);
	}

	(void)vfprintf(stream, format, args);
	(void)fprintf(stream, "\n");
}
