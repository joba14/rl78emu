
/**
 * @file cpu.c
 * 
 * @copyright This file is a part of the "rl78emu" project and is licensed, and
 * distributed under "rl78emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-03
 */

#include "rl78emu_misc/debug.h"
#include "rl78emu_misc/logger.h"

#include "rl78emu_core/regs.h"
#include "rl78emu_core/mem.h"
#include "rl78emu_core/cpu.h"

/**
 * note: regarding the instruction formats:
 * https://llvm-gcc-renesas.com/pdf/r01us0015ej0220_rl78.pdf @ 92 page.
 */

typedef struct
{
	const uint32_t flash_start_address;
	const uint32_t flash_length;

	const uint32_t cfm_start_address;
	const uint32_t cfm_length;

	const uint32_t sfr2_start_address;
	const uint32_t sfr2_length;

	const uint32_t dfm_start_address;
	const uint32_t dfm_length;

	const uint32_t ram_start_address;
	const uint32_t ram_length;

	// todo: add mirror areas (from and to).

	const uint32_t bank0_start_address;
	const uint32_t bank0_length;

	const uint32_t bank1_start_address;
	const uint32_t bank1_length;

	const uint32_t bank2_start_address;
	const uint32_t bank2_length;

	const uint32_t bank3_start_address;
	const uint32_t bank3_length;
} rl78core_config_s;

static rl78core_config_s g_rl78core_configs_table[rl78core_configs_count] =
{
	[rl78core_config_s1] =
	{
		.flash_start_address = 0x000000,
		.flash_length        = 0x100000,
		.cfm_start_address   = 0x000000,
		.cfm_length          = 0x0F0000,
		.sfr2_start_address  = 0x0F0000,
		.sfr2_length         = 0x000800,
		.dfm_start_address   = ,
		.dfm_length          = ,
		.ram_start_address   = 0x0FFEF8,
		.ram_length          = ,
		.bank0_start_address = ,
		.bank0_length        = ,
		.bank1_start_address = ,
		.bank1_length        = ,
		.bank2_start_address = ,
		.bank2_length        = ,
		.bank3_start_address = ,
		.bank3_length        = ,
	},
	[rl78core_config_s2] =
	{
	},
	[rl78core_config_s3] =
	{
	},
};

typedef struct
{
	rl78f14_reg20_u pc;
	rl78f14_reg08_u psw;
	rl78f14_reg16_u sp;  // note: the least significant bit must always be cleared to 0.
	rl78f14_reg08_u es;
	rl78f14_reg08_u cs;
} rl78core_cpu_s;

static rl78core_cpu_s g_rl78core_cpu;

void rl78core_cpu_init(
	const rl78core_config_e config)
{
	rl78emu_misc_debug_assert(config >= 0);
	rl78emu_misc_debug_assert(config < rl78core_configs_count);
	g_rl78core_cpu = ;  // &g_rl78core_cpu_table[type];
}
