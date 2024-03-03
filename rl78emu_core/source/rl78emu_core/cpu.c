
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
#include "rl78emu_core/cpu.h"
#include "rl78emu_core/mem.h"

typedef struct
{
	rl78f14_reg20_u pc;
	rl78f14_reg08_u psw;
	rl78f14_reg16_u sp; // note: the least significant bit must always be cleared to 0.
	rl78f14_reg08_u es;
	rl78f14_reg08_u cs;
} rl78f14_cpu_s;

/**
 * note: regarding the instruction formats:
 * https://llvm-gcc-renesas.com/pdf/r01us0015ej0220_rl78.pdf @ 92 page.
 */

static rl78f14_cpu_s g_rl78f14_cpu;

void rl78f14_cpu_init(
	void)
{
}

void rl78f14_cpu_reset(
	void)
{
	g_rl78f14_cpu.pc.value = rl78f14_mem_read_u16(0x0000);
	g_rl78f14_cpu.psw.value = 0x06;
	(void)g_rl78f14_cpu.sp.value; // undefined
	g_rl78f14_cpu.es.value = 0x0F;
	g_rl78f14_cpu.cs.value = 0x00;
	rl78emu_misc_logger_log("g_rl78f14_cpu.pc.value =%hu", g_rl78f14_cpu.pc.value );
	rl78emu_misc_logger_log("g_rl78f14_cpu.psw.value=%hu", g_rl78f14_cpu.psw.value);
	rl78emu_misc_logger_log("g_rl78f14_cpu.sp.value =%hu", g_rl78f14_cpu.sp.value );
	rl78emu_misc_logger_log("g_rl78f14_cpu.es.value =%hu", g_rl78f14_cpu.es.value );
	rl78emu_misc_logger_log("g_rl78f14_cpu.cs.value =%hu", g_rl78f14_cpu.cs.value );
}
