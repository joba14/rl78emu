
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

#include "rl78emu_core/reg.h"
#include "rl78emu_core/mem.h"
#include "rl78emu_core/cpu.h"

/**
 * note: regarding the instruction formats:
 * https://llvm-gcc-renesas.com/pdf/r01us0015ej0220_rl78.pdf @ 92 page.
 */

#define rl78core_gpr_x  0x00
#define rl78core_gpr_a  0x01
#define rl78core_gpr_ax 0x00

#define rl78core_gpr_c  0x02
#define rl78core_gpr_b  0x03
#define rl78core_gpr_bc 0x02

#define rl78core_gpr_e  0x04
#define rl78core_gpr_d  0x05
#define rl78core_gpr_de 0x04

#define rl78core_gpr_l  0x06
#define rl78core_gpr_h  0x07
#define rl78core_gpr_hl 0x06

typedef struct
{
	bool_t halted;

	rl78core_reg20_u pc;
	rl78core_reg08_u psw;  // +----+---+------+----+------+------+------+----+
						   // | IE | Z | RBS1 | AC | RBS0 | ISP1 | ISP0 | CY |
						   // +----+---+------+----+------+------+------+----+
	rl78core_reg16_u sp;  // note: the least significant bit must always be cleared to 0.
	rl78core_reg08_u es;
	rl78core_reg08_u cs;
} rl78core_cpu_s;

static rl78core_cpu_s g_rl78core_cpu;

// todo: short direct addressing (saddr) [0xFFE20; 0xFFF20) } 256 bytes
uint8_t short_direct_addressing_read_u08(const uint8_t address);
void short_direct_addressing_write_u08(const uint8_t address, const uint8_t value);

// todo: sfr addressing (sfr) [0xFFF00; 0x100000) } 256 bytes
uint8_t sfr_addressing_read_u08(const uint8_t address);
void sfr_addressing_write_u08(const uint8_t address, const uint8_t value);

// todo: register addressing [0xFFEE0; 0xFFF00) } 32 bytes
uint8_t gpr_addressing_read_u08(const uint8_t address);
void gpr_addressing_write_u08(const uint8_t address, const uint8_t value);

// todo: direct addressing [0x00000; 0x100000) } 1Mb
uint8_t direct_addressing_read_u08(const uint20_t address);
void direct_addressing_write_u08(const uint20_t address, const uint8_t value);

// todo: register indirect addressing [0x00000; 0x100000) } 1Mb
// todo: based addressing [0x00000; 0x100000) } 1Mb
// todo: based indexed addressing [0x00000; 0x100000) } 1Mb


static uint20_t get_current_bank_base_address(void);

static uint8_t read_general_purpose_register_u08(const uint8_t offset);

static void write_general_purpose_register_u08(const uint8_t offset, const uint8_t value);

static uint16_t read_general_purpose_register_u16(const uint8_t offset);

static void write_general_purpose_register_u16(const uint8_t offset, const uint16_t value);

static uint8_t fetch_instruction_byte(void);

void rl78core_cpu_init(void)
{
	g_rl78core_cpu = (rl78core_cpu_s)
	{
		.halted = false,
	};
}

bool_t rl78core_cpu_halted(void)
{
	return g_rl78core_cpu.halted;
}

void rl78core_cpu_tick(void)
{
	switch (fetch_instruction_byte())
	{
		case 0x50:  // MOV X, #byte
		{
			write_general_purpose_register_u08(rl78core_gpr_x, fetch_instruction_byte());
		} break;

		case 0x51:  // MOV A, #byte
		{
			write_general_purpose_register_u08(rl78core_gpr_a, fetch_instruction_byte());
		} break;

		case 0x52:  // MOV C, #byte
		{
			write_general_purpose_register_u08(rl78core_gpr_c, fetch_instruction_byte());
		} break;

		case 0x53:  // MOV B, #byte
		{
			write_general_purpose_register_u08(rl78core_gpr_b, fetch_instruction_byte());
		} break;

		case 0x54:  // MOV E, #byte
		{
			write_general_purpose_register_u08(rl78core_gpr_e, fetch_instruction_byte());
		} break;

		case 0x55:  // MOV D, #byte
		{
			write_general_purpose_register_u08(rl78core_gpr_d, fetch_instruction_byte());
		} break;

		case 0x56:  // MOV L, #byte
		{
			write_general_purpose_register_u08(rl78core_gpr_l, fetch_instruction_byte());
		} break;

		case 0x57:  // MOV H, #byte
		{
			write_general_purpose_register_u08(rl78core_gpr_h, fetch_instruction_byte());
		} break;


		case 0xCD:  // MOV saddr, #byte
		{
			// todo: handle addressing correctly - make wrappers in cpu!
			rl78core_mem_write_u08(fetch_instruction_byte(), fetch_instruction_byte());
		} break;

		case 0xCE:  // MOV sfr, #byte
		{
			// todo: handle addressing correctly - make wrappers in cpu!
		} break;

		case 0xCF:  // MOV !addr16, #byte
		{
			// todo: handle addressing correctly - make wrappers in cpu!
		} break;

		// -------------------------------------------------------- //

	}
}

static uint20_t get_current_bank_base_address(void)
{
	rl78emu_misc_debug_assert(!"todo: implement current bank fetching by the status flags!");
	return 0;
}

static uint8_t read_general_purpose_register_u08(const uint8_t offset)
{
	const uint20_t base = get_current_bank_base_address();
	return rl78core_mem_read_u08(base + offset);
}

static void write_general_purpose_register_u08(const uint8_t offset, const uint8_t value)
{
	const uint20_t base = get_current_bank_base_address();
	rl78core_mem_write_u08(base + offset, value);
	rl78emu_misc_debug_assert(!"todo: handle the flags and status register! ref: https://llvm-gcc-renesas.com/pdf/r01us0015ej0220_rl78.pdf");
}

static uint16_t read_general_purpose_register_u16(const uint8_t offset)
{
	const uint20_t base = get_current_bank_base_address();
	return rl78core_mem_read_u16(base + offset);
}

static void write_general_purpose_register_u16(const uint8_t offset, const uint16_t value)
{
	const uint20_t base = get_current_bank_base_address();
	rl78core_mem_write_u16(base + offset, value);
	rl78emu_misc_debug_assert(!"todo: handle the flags and status register! ref: https://llvm-gcc-renesas.com/pdf/r01us0015ej0220_rl78.pdf");
}

static uint8_t fetch_instruction_byte(void)
{
	return rl78core_mem_read_u08(g_rl78core_cpu.pc.value++);
}
