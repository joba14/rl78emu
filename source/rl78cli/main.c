
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

#include "rl78misc/logger.h"

#include "rl78core/mem.h"
#include "rl78core/cpu.h"

#include "rl78cli/config.h"

int32_t main(
	const int32_t argc,
	const char_t* argv[]);

int32_t main(
	const int32_t argc,
	const char_t* argv[])
{
	rl78misc_logger_log("rl78emu: hello, world!");

	const rl78cli_config_s config = rl78cli_config_from_cli((uint64_t)argc, argv);
	(void)config;

	rl78core_mem_init();
	rl78core_cpu_init();

	// todo: flash the binary into the mem:
	// [
		#define rom_length 2
		const uint8_t rom[rom_length] =
		{
			0x50, 0x69
		};

		for (uint20_t address = 0; address < rom_length; ++address)
		{
			rl78core_mem_write_u08(address, rom[address]);
		}
	// ]

	for (uint64_t tick_count = 0; !rl78core_cpu_halted(); ++tick_count)
	{
		rl78core_cpu_tick();
		rl78misc_logger_log("tick_count=%lu", tick_count);
		rl78misc_logger_log("----------");
	}

	return 0;
}
