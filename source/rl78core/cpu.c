
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

#include "rl78misc/debug.h"
#include "rl78misc/logger.h"

#include "rl78core/mem.h"
#include "rl78core/cpu.h"

/**
 * note: regarding the instruction formats:
 * https://llvm-gcc-renesas.com/pdf/r01us0015ej0220_rl78.pdf @ 92 page.
 */

typedef enum
{
	rl78core_fixed_sfr_spl = 0xFFFF8,  // note: the least significant bit must always be cleared to 0.
	rl78core_fixed_sfr_sph = 0xFFFF9,
	rl78core_fixed_sfr_psw = 0xFFFFA,  // +----+---+------+----+------+------+------+----+
									   // | IE | Z | RBS1 | AC | RBS0 | ISP1 | ISP0 | CY |
									   // +----+---+------+----+------+------+------+----+
	rl78core_fixed_sfr_cs = 0xFFFFC,
	rl78core_fixed_sfr_es = 0xFFFFD,
	rl78core_fixed_sfr_pmc = 0xFFFFE,
	rl78core_fixed_sfr_mem = 0xFFFFF,
} rl78core_fixed_sfr_e;

typedef struct
{
	bool_t halted;
	uint20_t pc;
} rl78core_cpu_s;

static rl78core_cpu_s g_rl78core_cpu;

/**
 * @brief Convert short direct address in the range of [0xFFE20; 0xFFF20) into
 * an absolute address in range of [0x00000; 0x100000).
 * 
 * @param address relative short address
 * 
 * @return uint20_t absolute address
 */
uint20_t short_direct_address_to_absolute_address(const uint8_t address);

/**
 * @brief Convert special function register in the range of [0xFFF00; 0x100000)
 * into an absolute address in range of [0x00000; 0x100000).
 * 
 * @param address relative special function register address
 * 
 * @return uint20_t absolute address
 */
uint20_t special_function_register_to_absolute_address(const uint8_t address);

/**
 * @brief Convert general purpose register in the range of [0xFFEE0; 0xFFF00)
 * into an absolute address in range of [0x00000; 0x100000).
 * 
 * @param offset general purpose register offset in registers bank
 * 
 * @return uint20_t absolute address
 */
uint20_t general_purpose_register_to_absolute_address(const uint8_t offset);

/**
 * @brief Convert direct address in the range of [0x00000; 0x100000) into an
 * absolute address in range of [0x00000; 0x100000).
 * 
 * @param address_low  lower 8 bits of the direct address
 * @param address_high higher 8 bits of the direct address
 * 
 * @return uint20_t absolute address
 */
uint20_t direct_address_to_absolute_address(const uint8_t address_low, const uint8_t address_high);

// todo: register indirect addressing [0x00000; 0x100000) } 1Mb
uint20_t indirect_register_address_to_absolute_address(const uint20_t address);

// todo: based addressing [0x00000; 0x100000) } 1Mb
uint20_t based_address_to_absolute_address(const uint20_t address);

// todo: based indexed addressing [0x00000; 0x100000) } 1Mb
uint20_t based_indexed_address_to_absolute_address(const uint20_t address);

/**
 * @brief Fetch instruction byte from the flash at pc register address.
 * 
 * @warning The pc register is incremented by one after the fetch.
 * 
 * @return uint8_t fetched byte
 */
static uint8_t fetch_instruction_byte(void);

void rl78core_cpu_init(void)
{
	g_rl78core_cpu = (rl78core_cpu_s)
	{
		.halted = false,
		.pc = 0x00000,
	};
}

uint20_t rl78core_cpu_read_pc(void)
{
	return g_rl78core_cpu.pc;
}

void rl78core_cpu_write_pc(const uint20_t value)
{
	g_rl78core_cpu.pc = value;
}

uint8_t rl78core_cpu_read_gpr08(const uint8_t gpr08)
{
	rl78misc_debug_assert(gpr08 < rl78core_gpr08s_count);
	const uint20_t address = general_purpose_register_to_absolute_address(gpr08);
	return rl78core_mem_read_u08(address);
}

void rl78core_cpu_write_gpr08(const uint8_t gpr08, const uint8_t value)
{
	rl78misc_debug_assert(gpr08 < rl78core_gpr08s_count);
	const uint20_t address = general_purpose_register_to_absolute_address(gpr08);
	rl78core_mem_write_u08(address, value);
	// todo: handle flags if needed!
}

uint16_t rl78core_cpu_read_gpr16(const uint8_t gpr16)
{
	rl78misc_debug_assert(0 == (gpr16 % 2));
	rl78misc_debug_assert(gpr16 < rl78core_gpr16s_count);
	const uint20_t address = general_purpose_register_to_absolute_address(gpr16);
	return rl78core_mem_read_u16(address);
}

void rl78core_cpu_write_gpr16(const uint8_t gpr16, const uint16_t value)
{
	rl78misc_debug_assert(0 == (gpr16 % 2));
	rl78misc_debug_assert(gpr16 < rl78core_gpr16s_count);
	const uint20_t address = general_purpose_register_to_absolute_address(gpr16);
	rl78core_mem_write_u16(address, value);
	// todo: handle flags if needed!
}

void rl78core_cpu_halt(void)
{
	g_rl78core_cpu.halted = true;
}

bool_t rl78core_cpu_halted(void)
{
	return g_rl78core_cpu.halted;
}

void rl78core_cpu_tick(void)
{
	if (rl78core_cpu_halted())
	{
		return;
	}

	switch (fetch_instruction_byte())
	{
		case 0x50:  // MOV X, #byte
		{
			const uint8_t data = fetch_instruction_byte();
			rl78core_cpu_write_gpr08(rl78core_gpr08_x, data);
		} break;

		case 0x51:  // MOV A, #byte
		{
			const uint8_t data = fetch_instruction_byte();
			rl78core_cpu_write_gpr08(rl78core_gpr08_a, data);
		} break;

		case 0x52:  // MOV C, #byte
		{
			const uint8_t data = fetch_instruction_byte();
			rl78core_cpu_write_gpr08(rl78core_gpr08_c, data);
		} break;

		case 0x53:  // MOV B, #byte
		{
			const uint8_t data = fetch_instruction_byte();
			rl78core_cpu_write_gpr08(rl78core_gpr08_b, data);
		} break;

		case 0x54:  // MOV E, #byte
		{
			const uint8_t data = fetch_instruction_byte();
			rl78core_cpu_write_gpr08(rl78core_gpr08_e, data);
		} break;

		case 0x55:  // MOV D, #byte
		{
			const uint8_t data = fetch_instruction_byte();
			rl78core_cpu_write_gpr08(rl78core_gpr08_d, data);
		} break;

		case 0x56:  // MOV L, #byte
		{
			const uint8_t data = fetch_instruction_byte();
			rl78core_cpu_write_gpr08(rl78core_gpr08_l, data);
		} break;

		case 0x57:  // MOV H, #byte
		{
			const uint8_t data = fetch_instruction_byte();
			rl78core_cpu_write_gpr08(rl78core_gpr08_h, data);
		} break;

#if 0
		case 0xCD:  // MOV saddr, #byte
		{
			const uint8_t saddr = fetch_instruction_byte();
			const uint8_t data = fetch_instruction_byte();
			const uint20_t absolute_address = short_direct_address_to_absolute_address(saddr);
			rl78core_mem_write_u08(absolute_address, data);
			// todo: handle flags if needed!
		} break;

		case 0xCE:  // MOV sfr, #byte
		{
			const uint8_t sfr = fetch_instruction_byte();
			const uint8_t data = fetch_instruction_byte();
			const uint20_t absolute_address = special_function_register_to_absolute_address(sfr);
			rl78core_mem_write_u08(absolute_address, data);
			// todo: handle flags if needed!
		} break;

		case 0xCF:  // MOV !addr16, #byte
		{
			const uint8_t addrl = fetch_instruction_byte();
			const uint8_t addrh = fetch_instruction_byte();
			const uint8_t data = fetch_instruction_byte();
			const uint20_t absolute_address = direct_address_to_absolute_address(addrl, addrh);
			rl78core_mem_write_u08(absolute_address, data);
			// todo: handle flags if needed!
		} break;

		case 0x60:  // MOV A, X
		{
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint20_t absolute_address_x = general_purpose_register_to_absolute_address(rl78core_gpr08_x);
			const uint8_t x_value = rl78core_mem_read_u08(absolute_address_x);
			rl78core_mem_write_u08(absolute_address_a, x_value);
			// todo: handle flags if needed!
		} break;

		case 0x62:  // MOV A, C
		{
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint20_t absolute_address_c = general_purpose_register_to_absolute_address(rl78core_gpr08_c);
			const uint8_t c_value = rl78core_mem_read_u08(absolute_address_c);
			rl78core_mem_write_u08(absolute_address_a, c_value);
			// todo: handle flags if needed!
		} break;

		case 0x63:  // MOV A, B
		{
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint20_t absolute_address_b = general_purpose_register_to_absolute_address(rl78core_gpr08_b);
			const uint8_t b_value = rl78core_mem_read_u08(absolute_address_b);
			rl78core_mem_write_u08(absolute_address_a, b_value);
			// todo: handle flags if needed!
		} break;

		case 0x64:  // MOV A, E
		{
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint20_t absolute_address_e = general_purpose_register_to_absolute_address(rl78core_gpr08_e);
			const uint8_t e_value = rl78core_mem_read_u08(absolute_address_e);
			rl78core_mem_write_u08(absolute_address_a, e_value);
			// todo: handle flags if needed!
		} break;

		case 0x65:  // MOV A, D
		{
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint20_t absolute_address_d = general_purpose_register_to_absolute_address(rl78core_gpr08_d);
			const uint8_t d_value = rl78core_mem_read_u08(absolute_address_d);
			rl78core_mem_write_u08(absolute_address_a, d_value);
			// todo: handle flags if needed!
		} break;

		case 0x66:  // MOV A, L
		{
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint20_t absolute_address_l = general_purpose_register_to_absolute_address(rl78core_gpr08_l);
			const uint8_t l_value = rl78core_mem_read_u08(absolute_address_l);
			rl78core_mem_write_u08(absolute_address_a, l_value);
			// todo: handle flags if needed!
		} break;

		case 0x67:  // MOV A, H
		{
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint20_t absolute_address_h = general_purpose_register_to_absolute_address(rl78core_gpr08_h);
			const uint8_t h_value = rl78core_mem_read_u08(absolute_address_h);
			rl78core_mem_write_u08(absolute_address_a, h_value);
			// todo: handle flags if needed!
		} break;

		case 0x70:  // MOV X, A
		{
			const uint20_t absolute_address_x = general_purpose_register_to_absolute_address(rl78core_gpr08_x);
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint8_t a_value = rl78core_mem_read_u08(absolute_address_a);
			rl78core_mem_write_u08(absolute_address_x, a_value);
			// todo: handle flags if needed!
		} break;

		case 0x72:  // MOV C, A
		{
			const uint20_t absolute_address_c = general_purpose_register_to_absolute_address(rl78core_gpr08_c);
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint8_t a_value = rl78core_mem_read_u08(absolute_address_a);
			rl78core_mem_write_u08(absolute_address_c, a_value);
			// todo: handle flags if needed!
		} break;

		case 0x73:  // MOV B, A
		{
			const uint20_t absolute_address_b = general_purpose_register_to_absolute_address(rl78core_gpr08_b);
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint8_t a_value = rl78core_mem_read_u08(absolute_address_a);
			rl78core_mem_write_u08(absolute_address_b, a_value);
			// todo: handle flags if needed!
		} break;

		case 0x74:  // MOV E, A
		{
			const uint20_t absolute_address_e = general_purpose_register_to_absolute_address(rl78core_gpr08_e);
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint8_t a_value = rl78core_mem_read_u08(absolute_address_a);
			rl78core_mem_write_u08(absolute_address_e, a_value);
			// todo: handle flags if needed!
		} break;

		case 0x75:  // MOV D, A
		{
			const uint20_t absolute_address_d = general_purpose_register_to_absolute_address(rl78core_gpr08_d);
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint8_t a_value = rl78core_mem_read_u08(absolute_address_a);
			rl78core_mem_write_u08(absolute_address_d, a_value);
			// todo: handle flags if needed!
		} break;

		case 0x76:  // MOV L, A
		{
			const uint20_t absolute_address_l = general_purpose_register_to_absolute_address(rl78core_gpr08_l);
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint8_t a_value = rl78core_mem_read_u08(absolute_address_a);
			rl78core_mem_write_u08(absolute_address_l, a_value);
			// todo: handle flags if needed!
		} break;

		case 0x77:  // MOV H, A
		{
			const uint20_t absolute_address_h = general_purpose_register_to_absolute_address(rl78core_gpr08_h);
			const uint20_t absolute_address_a = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint8_t a_value = rl78core_mem_read_u08(absolute_address_a);
			rl78core_mem_write_u08(absolute_address_h, a_value);
			// todo: handle flags if needed!
		} break;

		case 0x8D:  // MOV A, saddr
		{
			const uint8_t short_address = fetch_instruction_byte();
			uint20_t absolute_address = short_direct_address_to_absolute_address(short_address);
			const uint8_t data = rl78core_mem_read_u08(absolute_address);
			absolute_address = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			rl78core_mem_write_u08(absolute_address, data);
			// todo: handle flags if needed!
		} break;

		case 0x9D:  // MOV saddr, A
		{
			uint20_t absolute_address = general_purpose_register_to_absolute_address(rl78core_gpr08_a);
			const uint8_t data = rl78core_mem_read_u08(absolute_address);
			const uint8_t short_address = fetch_instruction_byte();
			absolute_address = short_direct_address_to_absolute_address(short_address);
			rl78core_mem_write_u08(absolute_address, data);
			// todo: handle flags if needed!
		} break;
#endif

		// -------------------------------------------------------- //

		// -------------------------------------------------------- //

		default:
		{
			g_rl78core_cpu.halted = true;
		} break;
	}
}

uint20_t short_direct_address_to_absolute_address(const uint8_t address)
{
	const uint20_t short_direct_addressing_start = 0xFFE20;
	const uint16_t short_direct_addressing_length = 0x100;

	rl78misc_debug_assert((short_direct_addressing_start + address) < \
		(short_direct_addressing_start + short_direct_addressing_length)
	);

	const uint20_t absolute_address = short_direct_addressing_start + address;
	return absolute_address;
}

uint20_t special_function_register_to_absolute_address(const uint8_t address)
{
	const uint20_t special_function_register_start = 0xFFF00;
	const uint16_t special_function_register_length = 0x100;

	rl78misc_debug_assert((special_function_register_start + address) < \
		(special_function_register_start + special_function_register_length)
	);

	const uint20_t absolute_address = special_function_register_start + address;
	return absolute_address;
}

uint20_t general_purpose_register_to_absolute_address(const uint8_t offset)
{
	const uint8_t  general_purpose_register_count_per_bank = 8;
	const uint20_t general_purpose_register_start = 0xFFEE0;
	const uint8_t  general_purpose_register_length = 0x20;

	rl78misc_debug_assert(offset < general_purpose_register_count_per_bank);
	const uint8_t psw_value = rl78core_mem_read_u08(rl78core_fixed_sfr_psw);
	// +----+---+------+----+------+------+------+----+      +------+------+
	// | IE | Z | RBS1 | AC | RBS0 | ISP1 | ISP0 | CY |  ->  | RBS1 | RBS0 |
	// +----+---+------+----+------+------+------+----+      +------+------+
	const uint8_t current_gpr_bank = (uint8_t)(
		(uint8_t)((uint8_t)(psw_value & 0x08) >> 3) |
		(uint8_t)((uint8_t)(psw_value & 0x20) >> 4)
	);
	const uint8_t address = (uint8_t)((uint8_t)(current_gpr_bank * general_purpose_register_count_per_bank) + offset);

	rl78misc_debug_assert((general_purpose_register_start + address) < \
		(general_purpose_register_start + general_purpose_register_length)
	);

	const uint20_t absolute_address = general_purpose_register_start + address;
	return absolute_address;
}

uint20_t direct_address_to_absolute_address(const uint8_t address_low, const uint8_t address_high)
{
	const uint8_t es_value = rl78core_mem_read_u08(rl78core_fixed_sfr_es);
	// +---+---+---+---+-----+-----+-----+-----+      +-----+-----+-----+-----+
	// | 0 | 0 | 0 | 0 | ES3 | ES2 | ES1 | ES0 |  ->  | ES3 | ES2 | ES1 | ES0 |
	// +---+---+---+---+-----+-----+-----+-----+      +-----+-----+-----+-----+
	const uint20_t absolute_address = (uint20_t)(
		(uint20_t)(address_low & 0xFF) |
		(uint20_t)((uint20_t)(address_high & 0xFF) << 8) |
		(uint20_t)((uint20_t)(es_value & 0x0F) << 16)
	);
	return absolute_address;
}

uint20_t indirect_register_address_to_absolute_address(const uint20_t address)
{
	(void)address;
	return 0;
}

uint20_t based_address_to_absolute_address(const uint20_t address)
{
	(void)address;
	return 0;
}

uint20_t based_indexed_address_to_absolute_address(const uint20_t address)
{
	(void)address;
	return 0;
}

static uint8_t fetch_instruction_byte(void)
{
	return rl78core_mem_read_u08(g_rl78core_cpu.pc++);
}
