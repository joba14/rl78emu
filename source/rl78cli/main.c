
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

#include "rl78misc/debug.h"
#include "rl78misc/logger.h"

#include "rl78core/mem.h"
#include "rl78core/cpu.h"

#include "rl78cli/config.h"
#include "rl78cli/main.h"

#include <stdio.h>

static void display_cpu_state(const uint64_t tick);

static void flash_rom_to_mem(const uint8_t* const rom, const uint20_t length);

int32_t main(const int32_t argc, const char_t* argv[])
{
	rl78misc_logger_log("rl78emu: hello, world!");

	const rl78cli_config_s config = rl78cli_config_from_cli((uint64_t)argc, argv);
	(void)config;

	rl78core_mem_init();
	rl78core_cpu_init();

	// todo: flash the binary into the mem:
	// const uint8_t rom[] = { 0x50, 0x69, 0xCE, 0xFA, 0x0A, 0x00, 0x51, 0x05, 0x00, 0x00, 0x01 };
	const uint8_t rom[] =
	{
		0x50, 0x69,        // MOV X, 0x69
		0x51, 0x05,        // MOV A, 0x05
		0x50, 0x20,        // MOV X, 0x20
		0x51, 0x20,         // MOV A, 0x20
		0x61, 0xED         // HALT
	};
	#define rom_length (sizeof(rom) / sizeof(rom[0]))
	flash_rom_to_mem(rom, rom_length);

	for (uint64_t tick_count = 0; !rl78core_cpu_halted(); ++tick_count)
	{
		rl78misc_logger_log("old state:");
		rl78core_cpu_tick();
		rl78misc_logger_log("new state:");
		display_cpu_state(tick_count);
		getchar();
	}

	return 0;
}

static void display_cpu_state(const uint64_t tick)
{
	rl78core_cpu_stats_s stats;
	rl78core_cpu_get_stats(&stats);

	rl78misc_logger_log("cpu state:");
	rl78misc_logger_log("    tick= %lu", tick);
	rl78misc_logger_log("    pc  = 0x%08X (%u)", stats.pc,  stats.pc );
	rl78misc_logger_log("    x   = 0x%08X (%u)", stats.x,   stats.x  );
	rl78misc_logger_log("    a   = 0x%08X (%u)", stats.a,   stats.a  );
	rl78misc_logger_log("    c   = 0x%08X (%u)", stats.c,   stats.c  );
	rl78misc_logger_log("    b   = 0x%08X (%u)", stats.b,   stats.b  );
	rl78misc_logger_log("    e   = 0x%08X (%u)", stats.e,   stats.e  );
	rl78misc_logger_log("    d   = 0x%08X (%u)", stats.d,   stats.d  );
	rl78misc_logger_log("    l   = 0x%08X (%u)", stats.l,   stats.l  );
	rl78misc_logger_log("    h   = 0x%08X (%u)", stats.h,   stats.h  );
	rl78misc_logger_log("    ax  = 0x%08X (%u)", stats.ax,  stats.ax );
	rl78misc_logger_log("    bc  = 0x%08X (%u)", stats.bc,  stats.bc );
	rl78misc_logger_log("    de  = 0x%08X (%u)", stats.de,  stats.de );
	rl78misc_logger_log("    hl  = 0x%08X (%u)", stats.hl,  stats.hl );
	rl78misc_logger_log("    spl = 0x%08X (%u)", stats.spl, stats.spl);
	rl78misc_logger_log("    sph = 0x%08X (%u)", stats.sph, stats.sph);
	rl78misc_logger_log("    psw = 0x%08X (%u)", stats.psw, stats.psw);
	rl78misc_logger_log("    cs  = 0x%08X (%u)", stats.cs,  stats.cs );
	rl78misc_logger_log("    es  = 0x%08X (%u)", stats.es,  stats.es );
	rl78misc_logger_log("    pmc = 0x%08X (%u)", stats.pmc, stats.pmc);
	rl78misc_logger_log("    mem = 0x%08X (%u)", stats.mem, stats.mem);
	rl78misc_logger_log("---------");
}

static void flash_rom_to_mem(const uint8_t* const rom, const uint20_t length)
{
	rl78misc_debug_assert(rom != NULL);
	rl78misc_debug_assert(length > 0);

	for (uint20_t address = 0; address < length; ++address)
	{
		rl78core_mem_write_u08(address, rom[address]);
	}
}
