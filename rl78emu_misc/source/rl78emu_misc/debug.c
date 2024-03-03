
/**
 * @file debug.c
 * 
 * @copyright This file is a part of the "rl78emu" project and is licensed, and
 * distributed under "rl78emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-03
 */

#include "rl78emu_misc/debug.h"

#ifndef NDEBUG
#	include <stdlib.h>
#	include <stdio.h>

void rl78emu_misc_debug_assert_impl(
	const bool_t expression,
	const char_t* const expression_string,
	const char_t* const file,
	const uint64_t line)
{
	if (!expression)
	{
		(void)fprintf(stderr,
			"\033[91m" "\033[1m" "%s" "\033[0m" ": assertion '%s' failed at %s:%lu\n",
			"fatal", expression_string, file, line);
		rl78emu_misc_exit(-1);
	}
}
#else
_Static_assert(1, "");  // note: to prevent empty translation unit error.
#endif
