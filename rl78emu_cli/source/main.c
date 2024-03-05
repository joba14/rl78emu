
/**
 * @file main.c
 * 
 * @copyright This file is a part of the "rl78emu" project and is licensed, and
 * distributed under "rl78emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-03
 */

#include "rl78emu_misc/logger.h"

#include "rl78emu_core/reg.h"
#include "rl78emu_core/mem.h"
#include "rl78emu_core/cpu.h"

#include "rl78emu_cli/config.h"

int32_t main(
	const int32_t argc,
	const char_t* argv[]);

int32_t main(
	const int32_t argc,
	const char_t* argv[])
{
	rl78emu_misc_logger_log("rl78emu_cli: hello, world!");

#if 1
	(void)argc;
	(void)argv;
#else
	// todo: uncomment:
	// const rl78emu_cli_config_s config = rl78emu_cli_config_from_cli((uint64_t)argc, argv);
	// (void)config;
#endif

	rl78core_mem_init();
	rl78core_cpu_init();
	// todo: flash the binary into the mem.

	while (!rl78core_cpu_halted())
	{
		rl78core_cpu_tick();
	}

	return 0;
}
