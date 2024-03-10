
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

typedef struct
{
	#define rl78core_ins_max_size 5
	uint8_t buffer[rl78core_ins_max_size];
	bool_t fetched;
} rl78core_fetch_s;

typedef struct
{
	uint32_t opcode;
	uint8_t cycles;
	uint8_t length;
	bool_t decoded;

	uint20_t address;
	uint8_t data;
} rl78core_ins_s;

typedef struct
{
	rl78core_fetch_s fetch;
	rl78core_ins_s instruction;

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
/* static */ uint20_t short_direct_address_to_absolute_address(const uint8_t address);

/**
 * @brief Convert special function register in the range of [0xFFF00; 0x100000)
 * into an absolute address in range of [0x00000; 0x100000).
 * 
 * @param address relative special function register address
 * 
 * @return uint20_t absolute address
 */
static uint20_t special_function_register_to_absolute_address(const uint8_t address);

/**
 * @brief Convert general purpose register in the range of [0xFFEE0; 0xFFF00)
 * into an absolute address in range of [0x00000; 0x100000).
 * 
 * @param offset general purpose register offset in registers bank
 * 
 * @return uint20_t absolute address
 */
static uint20_t general_purpose_register_to_absolute_address(const uint8_t offset);

/**
 * @brief Convert direct address in the range of [0x00000; 0x100000) into an
 * absolute address in range of [0x00000; 0x100000).
 * 
 * @param address_low  lower 8 bits of the direct address
 * @param address_high higher 8 bits of the direct address
 * 
 * @return uint20_t absolute address
 */
/* static */ uint20_t direct_address_to_absolute_address(const uint8_t address_low, const uint8_t address_high);

// todo: register indirect addressing [0x00000; 0x100000) } 1Mb
/* static */ uint20_t indirect_register_address_to_absolute_address(const uint20_t address);

// todo: based addressing [0x00000; 0x100000) } 1Mb
/* static */ uint20_t based_address_to_absolute_address(const uint20_t address);

// todo: based indexed addressing [0x00000; 0x100000) } 1Mb
/* static */ uint20_t based_indexed_address_to_absolute_address(const uint20_t address);

/**
 * @brief Fetch instruction byte from the flash at pc register address.
 * 
 * @warning The pc register is incremented by one after the fetch.
 * 
 * @return uint8_t fetched byte
 */
static uint8_t fetch_instruction_byte(void);

/**
 * @brief Fetch instruction from the flash (5 bytes).
 * 
 * @warning The pc register is incremented by 5. The pc will be updated in the
 * decoding stage to not over-step the actual instruction.
 */
static void fetch_instruction(void);

/**
 * @brief Decode instruction from the 5 fetched bytes.
 */
static void decode_instruction(void);

/**
 * @brief Execute the decoded instruction.
 */
static void execute_instruction(void);

void rl78core_cpu_init(void)
{
	g_rl78core_cpu = (const rl78core_cpu_s)
	{
		.fetch       = (const rl78core_fetch_s) {0},
		.instruction = (const rl78core_ins_s)   {0},
		.halted      = false,
		.pc          = 0x00000,
	};
}

void rl78core_cpu_halt(void)
{
	g_rl78core_cpu.halted = true;
}

bool_t rl78core_cpu_halted(void)
{
	return g_rl78core_cpu.halted;
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
	rl78misc_debug_assert(gpr08 < rl78core_gprs_count);
	const uint20_t address = general_purpose_register_to_absolute_address(gpr08);
	return rl78core_mem_read_u08(address);
}

void rl78core_cpu_write_gpr08(const uint8_t gpr08, const uint8_t value)
{
	rl78misc_debug_assert(gpr08 < rl78core_gprs_count);
	const uint20_t address = general_purpose_register_to_absolute_address(gpr08);
	rl78core_mem_write_u08(address, value);
	// todo: handle flags if needed!
}

uint16_t rl78core_cpu_read_gpr16(const uint8_t gpr16)
{
	rl78misc_debug_assert(0 == (gpr16 % 2));
	rl78misc_debug_assert(gpr16 < rl78core_gprs_count);
	const uint20_t address = general_purpose_register_to_absolute_address(gpr16);
	return rl78core_mem_read_u16(address);
}

void rl78core_cpu_write_gpr16(const uint8_t gpr16, const uint16_t value)
{
	rl78misc_debug_assert(0 == (gpr16 % 2));
	rl78misc_debug_assert(gpr16 < rl78core_gprs_count);
	const uint20_t address = general_purpose_register_to_absolute_address(gpr16);
	rl78core_mem_write_u16(address, value);
	// todo: handle flags if needed!
}

uint8_t rl78core_cpu_read_sfr08(const uint8_t sfr08)
{
	// note: do not need to assert the sfr08 bounds as there are 256 sfrs.
	const uint20_t address = special_function_register_to_absolute_address(sfr08);
	return rl78core_mem_read_u08(address);
}

void rl78core_cpu_write_sfr08(const uint8_t sfr08, const uint8_t value)
{
	// note: do not need to assert the sfr08 bounds as there are 256 sfrs.
	const uint20_t address = special_function_register_to_absolute_address(sfr08);
	rl78core_mem_write_u08(address, value);
	// todo: handle flags if needed!
}

uint16_t rl78core_cpu_read_sfr16(const uint8_t sfr16)
{
	rl78misc_debug_assert(0 == (sfr16 % 2));
	// note: do not need to assert the sfr08 bounds as there are 256 sfrs.
	const uint20_t address = special_function_register_to_absolute_address(sfr16);
	return rl78core_mem_read_u16(address);
}

void rl78core_cpu_write_sfr16(const uint8_t sfr16, const uint16_t value)
{
	rl78misc_debug_assert(0 == (sfr16 % 2));
	// note: do not need to assert the sfr08 bounds as there are 256 sfrs.
	const uint20_t address = special_function_register_to_absolute_address(sfr16);
	rl78core_mem_write_u16(address, value);
	// todo: handle flags if needed!
}

void rl78core_cpu_get_stats(rl78core_cpu_stats_s* const stats)
{
	rl78misc_debug_assert(stats != NULL);

	stats->pc = rl78core_cpu_read_pc();

	stats->x  = rl78core_cpu_read_gpr08(rl78core_gpr08_x);
	stats->a  = rl78core_cpu_read_gpr08(rl78core_gpr08_a);
	stats->c  = rl78core_cpu_read_gpr08(rl78core_gpr08_c);
	stats->b  = rl78core_cpu_read_gpr08(rl78core_gpr08_b);
	stats->e  = rl78core_cpu_read_gpr08(rl78core_gpr08_e);
	stats->d  = rl78core_cpu_read_gpr08(rl78core_gpr08_d);
	stats->l  = rl78core_cpu_read_gpr08(rl78core_gpr08_l);
	stats->h  = rl78core_cpu_read_gpr08(rl78core_gpr08_h);
	stats->ax = rl78core_cpu_read_gpr16(rl78core_gpr16_ax);
	stats->bc = rl78core_cpu_read_gpr16(rl78core_gpr16_bc);
	stats->de = rl78core_cpu_read_gpr16(rl78core_gpr16_de);
	stats->hl = rl78core_cpu_read_gpr16(rl78core_gpr16_hl);

	stats->spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl);
	stats->sph = rl78core_cpu_read_sfr08(rl78core_sfr08_sph);
	stats->psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw);
	stats->cs  = rl78core_cpu_read_sfr08(rl78core_sfr08_cs);
	stats->es  = rl78core_cpu_read_sfr08(rl78core_sfr08_es);
	stats->pmc = rl78core_cpu_read_sfr08(rl78core_sfr08_pmc);
	stats->mem = rl78core_cpu_read_sfr08(rl78core_sfr08_mem);
}

void rl78core_cpu_tick(void)
{
	if (g_rl78core_cpu.halted)
	{
		return;
	}

	// 1. MEM (memory access): Decoded instruction is executed and memory at target address is accessed.
	execute_instruction();

	if (g_rl78core_cpu.halted)
	{
		return;
	}

	// 2, ID (instruction decode): Instruction is decoded and address is calculated.
	decode_instruction();

	if (g_rl78core_cpu.halted)
	{
		return;
	}

	// 3. IF (instruction fetch): Instruction is fetched and fetch pointer is incremented.
	fetch_instruction();
}

/* static */ uint20_t short_direct_address_to_absolute_address(const uint8_t address)
{
	const uint20_t short_direct_addressing_start = 0xFFE20;
	const uint16_t short_direct_addressing_length = 0x100;

	rl78misc_debug_assert((short_direct_addressing_start + address) <
		(short_direct_addressing_start + short_direct_addressing_length)
	);

	const uint20_t absolute_address = short_direct_addressing_start + address;
	return absolute_address;
}

static uint20_t special_function_register_to_absolute_address(const uint8_t address)
{
	const uint20_t special_function_register_start = 0xFFF00;
	const uint16_t special_function_register_length = 0x100;

	rl78misc_debug_assert((special_function_register_start + address) <
		(special_function_register_start + special_function_register_length)
	);

	const uint20_t absolute_address = special_function_register_start + address;
	return absolute_address;
}

static uint20_t general_purpose_register_to_absolute_address(const uint8_t offset)
{
	const uint8_t  general_purpose_register_count_per_bank = 8;
	const uint20_t general_purpose_register_start = 0xFFEE0;
	const uint8_t  general_purpose_register_length = 0x20;

	rl78misc_debug_assert(offset < general_purpose_register_count_per_bank);
	const uint8_t psw_value = rl78core_cpu_read_sfr08(rl78core_sfr08_psw);
	// +----+---+------+----+------+------+------+----+      +------+------+
	// | IE | Z | RBS1 | AC | RBS0 | ISP1 | ISP0 | CY |  ->  | RBS1 | RBS0 |
	// +----+---+------+----+------+------+------+----+      +------+------+
	const uint8_t current_gpr_bank = (uint8_t)(
		(uint8_t)((uint8_t)(psw_value & 0x08) >> 3) |
		(uint8_t)((uint8_t)(psw_value & 0x20) >> 4)
	);

	const uint8_t address = (uint8_t)(
		(uint8_t)(current_gpr_bank * general_purpose_register_count_per_bank) + offset
	);

	rl78misc_debug_assert((general_purpose_register_start + address) <
		(general_purpose_register_start + general_purpose_register_length)
	);

	const uint20_t absolute_address = general_purpose_register_start + address;
	return absolute_address;
}

/* static */ uint20_t direct_address_to_absolute_address(const uint8_t address_low, const uint8_t address_high)
{
	const uint8_t es_value = rl78core_cpu_read_sfr08(rl78core_sfr08_es);
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

/* static */ uint20_t indirect_register_address_to_absolute_address(const uint20_t address)
{
	(void)address;
	return 0;
}

/* static */ uint20_t based_address_to_absolute_address(const uint20_t address)
{
	(void)address;
	return 0;
}

/* static */ uint20_t based_indexed_address_to_absolute_address(const uint20_t address)
{
	(void)address;
	return 0;
}

static uint8_t fetch_instruction_byte(void)
{
	return rl78core_mem_read_u08(g_rl78core_cpu.pc++);
}

static void fetch_instruction(void)
{
	g_rl78core_cpu.fetch.fetched = false;

	for (uint8_t index = 0; index < rl78core_ins_max_size; ++index)
	{
		g_rl78core_cpu.fetch.buffer[index] = fetch_instruction_byte();
	}

	g_rl78core_cpu.fetch.fetched = true;
}

static void decode_instruction(void)
{
	if (!g_rl78core_cpu.fetch.fetched)
	{
		g_rl78core_cpu.instruction.decoded = false;
		return;
	}

	const uint8_t first_byte  = g_rl78core_cpu.fetch.buffer[0];
	const uint8_t second_byte = g_rl78core_cpu.fetch.buffer[1];

	switch (first_byte)
	{
		case 0x00:  // NOP
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode = first_byte,
				.length = 1,
				.cycles = 1,
			};
		} break;

		case 0x50:  // MOV X, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = general_purpose_register_to_absolute_address(rl78core_gpr08_x),
				.data    = second_byte,
				.length  = 2,
				.cycles  = 1,
			};
		} break;

		case 0x51:  // MOV A, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = general_purpose_register_to_absolute_address(rl78core_gpr08_a),
				.data    = second_byte,
				.length  = 2,
				.cycles  = 1,
			};
		} break;

		case 0x52:  // MOV C, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = general_purpose_register_to_absolute_address(rl78core_gpr08_c),
				.data    = second_byte,
				.length  = 2,
				.cycles  = 1,
			};
		} break;

		case 0x53:  // MOV B, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = general_purpose_register_to_absolute_address(rl78core_gpr08_b),
				.data    = second_byte,
				.length  = 2,
				.cycles  = 1,
			};
		} break;

		case 0x54:  // MOV E, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = general_purpose_register_to_absolute_address(rl78core_gpr08_e),
				.data    = second_byte,
				.length  = 2,
				.cycles  = 1,
			};
		} break;

		case 0x55:  // MOV D, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = general_purpose_register_to_absolute_address(rl78core_gpr08_d),
				.data    = second_byte,
				.length  = 2,
				.cycles  = 1,
			};
		} break;

		case 0x56:  // MOV L, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = general_purpose_register_to_absolute_address(rl78core_gpr08_l),
				.data    = second_byte,
				.length  = 2,
				.cycles  = 1,
			};
		} break;

		case 0x57:  // MOV H, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = general_purpose_register_to_absolute_address(rl78core_gpr08_h),
				.data    = second_byte,
				.length  = 2,
				.cycles  = 1,
			};
		} break;

		default:
		{
			g_rl78core_cpu.instruction.decoded = false;
			g_rl78core_cpu.halted = true;
			return;
		} break;
	}

	g_rl78core_cpu.pc -= (rl78core_ins_max_size - g_rl78core_cpu.instruction.length);
	g_rl78core_cpu.instruction.decoded = true;
}

static void execute_instruction(void)
{
	if (!g_rl78core_cpu.instruction.decoded)
	{
		return;
	}

	switch (g_rl78core_cpu.instruction.opcode)
	{
		case 0x00:  // NOP
		{
		} break;

		case 0x50:  // MOV X, #byte
		case 0x51:  // MOV A, #byte
		case 0x52:  // MOV C, #byte
		case 0x53:  // MOV B, #byte
		case 0x54:  // MOV E, #byte
		case 0x55:  // MOV D, #byte
		case 0x56:  // MOV L, #byte
		case 0x57:  // MOV H, #byte
		{
			rl78core_mem_write_u08(g_rl78core_cpu.instruction.address, g_rl78core_cpu.instruction.data);
		} break;

		default:
		{
			g_rl78core_cpu.halted = true;
			return;
		} break;
	}
}
