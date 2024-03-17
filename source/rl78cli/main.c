
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

static void flash_rom_to_mem(const uint8_t* const rom, const uint20_t length);

static void display_cpu_state(const uint64_t tick);

int32_t main(const int32_t argc, const char_t* argv[])
{
	rl78misc_logger_log("rl78emu: hello, world!");

	const rl78cli_config_s config = rl78cli_config_from_cli((uint64_t)argc, argv);
	(void)config;

	rl78core_mem_init();
	rl78core_cpu_init();

	// todo: flash the binary into the mem:
	// const uint8_t rom[] = { 0x50, 0x69, 0xCE, 0xFA, 0x0A, 0x00, 0x51, 0x05, 0x00, 0x00, 0x01 };
	// const uint8_t rom[] =
	// {
	// 	0x50, 0x69,        // MOV X, 0x69
	// 	0x51, 0x05,        // MOV A, 0x05
	// 	0xCE, 0xF8, 0x0A,  // MOV sfr, 0x0A
	// 	0x73,              // MOV B, A
	// 	0x50, 0x20,        // MOV X, 0x20
	// 	0x51, 0x20,        // MOV A, 0x20
	// 	0x61, 0xED         // HALT
	// };
	const uint8_t rom[] =
	{
		0x11, 0xCF, 0x0A, 0x00, 0x0A,  // MOV ES:addr16(0x'00'0A), 0x0A
		0x11, 0xCF, 0x0A, 0x00, 0x00,  // MOV ES:addr16(0x'00'0A), 0x00
		0x11, 0xCF, 0x0A, 0x00, 0x0A,  // MOV ES:addr16(0x'00'0A), 0x0A
		0x11, 0xCF, 0x0A, 0x00, 0x00,  // MOV ES:addr16(0x'00'0A), 0x00
	};
	#define rom_length (sizeof(rom) / sizeof(rom[0]))
	flash_rom_to_mem(rom, rom_length);

	for (uint64_t tick = 0; !rl78core_cpu_halted(); ++tick)
	{
		rl78core_cpu_tick();
		display_cpu_state(tick);
		getchar();
	}

	return 0;
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

static void display_cpu_state(const uint64_t tick)
{
	typedef struct
	{
		uint20_t pc;
		uint8_t x, a, c, b, e, d, l, h;
		uint16_t ax, bc, de, hl;
		uint8_t spl, sph, psw, cs, es, pmc, mem;
	} stats_s;

	stats_s stats =
	{
		.pc  = rl78core_cpu_pc_read(),
		.x   = rl78core_mem_read_u08(rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_x)),
		.a   = rl78core_mem_read_u08(rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_a)),
		.c   = rl78core_mem_read_u08(rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_c)),
		.b   = rl78core_mem_read_u08(rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_b)),
		.e   = rl78core_mem_read_u08(rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_e)),
		.d   = rl78core_mem_read_u08(rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_d)),
		.l   = rl78core_mem_read_u08(rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_l)),
		.h   = rl78core_mem_read_u08(rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_h)),
		.ax  = rl78core_mem_read_u16(rl78core_cpu_gpr16_to_absolute_address(rl78core_gpr16_ax)),
		.bc  = rl78core_mem_read_u16(rl78core_cpu_gpr16_to_absolute_address(rl78core_gpr16_bc)),
		.de  = rl78core_mem_read_u16(rl78core_cpu_gpr16_to_absolute_address(rl78core_gpr16_de)),
		.hl  = rl78core_mem_read_u16(rl78core_cpu_gpr16_to_absolute_address(rl78core_gpr16_hl)),
		.spl = rl78core_mem_read_u08(rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_spl)),
		.sph = rl78core_mem_read_u08(rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_sph)),
		.psw = rl78core_mem_read_u08(rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_psw)),
		.cs  = rl78core_mem_read_u08(rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_cs )),
		.es  = rl78core_mem_read_u08(rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_es )),
		.pmc = rl78core_mem_read_u08(rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_pmc)),
		.mem = rl78core_mem_read_u08(rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_mem)),
	};

	rl78misc_logger_log("cpu state:");
	rl78misc_logger_log("    tick= 0x%08lX (%lu)", tick, tick);
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
