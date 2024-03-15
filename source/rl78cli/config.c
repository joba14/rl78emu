
/**
 * @file config.c
 * 
 * @copyright This file's a part of the "rl78f14emu" project and is distributed
 * under "rl78f14emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-02
 */

#include "rl78misc/debug.h"
#include "rl78misc/logger.h"

#include "rl78cli/config.h"

#include "rl78emu/version.h"

static const char_t* g_program = NULL;
static const char_t* const g_usage_banner =
	"usage: %s [options] <binary>\n"
	"\n"
	"required:\n"
	"    binary              binary file path to be flashed into the emulator and run.\n"
	"                        it can be one of the following types: [ihex|srec].\n"
	"                        if binary path is not provided, emulator will exit and print this message.\n"
	"                        note: the srec format will not be supported for the first release.\n"
	"\n"
	"options:\n"
	"    -h, --help          print the help message.\n"
	"    -v, --version       print version and exit.\n"
	"\n"
	"notice:\n"
	"    this executable is distributed under the \"rl78f14emu gplv1\" license.\n";

static bool_t match_option(const char_t* const option, const char_t* const long_name, const char_t* const short_name) nodiscard;

rl78cli_config_s rl78cli_config_from_cli(const uint64_t argc, const char_t** const argv)
{
	rl78misc_debug_assert(argc > 0);
	rl78misc_debug_assert(argv != NULL);

	g_program = argv[0];
	const char_t* binary = NULL;

	for (uint64_t argv_index = 1; argv_index < argc; ++argv_index)
	{
		const char_t* const option = argv[argv_index];
		rl78misc_debug_assert(option != NULL);

		if (match_option(option, "--help", "-h"))
		{
			rl78cli_config_usage();
			rl78misc_exit(0);
		}
		else if (match_option(option, "--version", "-v"))
		{
			rl78misc_logger_log("%s %s", g_program, rl78emu_version);
			rl78misc_exit(0);
		}
		else
		{
			if (binary != NULL)
			{
				rl78misc_logger_error("binary path was already provided: '%s'. invalid command line option '%s' was provided.", binary, option);
				rl78cli_config_usage();
				rl78misc_exit(-1);
			}

			binary = option;
		}
	}

	if (NULL == binary)
	{
		rl78misc_logger_error("missing required binary path argument.");
		rl78cli_config_usage();
		rl78misc_exit(-1);
	}

	return (rl78cli_config_s)
	{
		.binary = binary,
	};
}

void rl78cli_config_usage(void)
{
	rl78misc_debug_assert(g_usage_banner != NULL);
	rl78misc_debug_assert(g_program != NULL);
	rl78misc_logger_log(g_usage_banner, g_program);
}

static bool_t match_option(const char_t* const option, const char_t* const long_name, const char_t* const short_name)
{
	rl78misc_debug_assert(option != NULL);
	rl78misc_debug_assert(long_name != NULL);
	rl78misc_debug_assert(short_name != NULL);

	const uint64_t option_length = rl78misc_strlen(option);
	const uint64_t long_name_length = rl78misc_strlen(long_name);
	const uint64_t short_name_length = rl78misc_strlen(short_name);

	return (
		((option_length ==  long_name_length) && (rl78misc_strncmp(option, long_name,  option_length) == 0)) ||
		((option_length == short_name_length) && (rl78misc_strncmp(option, short_name, option_length) == 0))
	);
}
