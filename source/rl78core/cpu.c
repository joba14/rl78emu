
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

// todo: define an enum or defines - it is near impossible to safely test and
//       implement new instructions using hex literals as the ids for the
//       instructions.

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
static uint20_t short_direct_address_to_absolute_address(const uint8_t address);

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
static uint20_t direct_address_to_absolute_address(const uint8_t address_low, const uint8_t address_high);

// todo: register indirect addressing [0x00000; 0x100000) } 1Mb
/* static */ uint20_t indirect_register_address_to_absolute_address(const uint20_t address);

// todo: based addressing [0x00000; 0x100000) } 1Mb
/* static */ uint20_t based_address_to_absolute_address(const uint20_t address);

// todo: based indexed addressing [0x00000; 0x100000) } 1Mb
/* static */ uint20_t based_indexed_address_to_absolute_address(const uint20_t address);

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
	rl78core_cpu_reset();
}

void rl78core_cpu_reset(void)
{
	g_rl78core_cpu = (const rl78core_cpu_s)
	{
		.fetch       = (const rl78core_fetch_s) {0},
		.instruction = (const rl78core_ins_s)   {0},
		.halted      = false,
		.pc          = 0x00000,
	};

	// todo: finish implementing!
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

uint8_t rl78core_cpu_read_saddr(const uint8_t saddr)
{
	// note: do not need to assert the saddr bounds as there are 256 saddrs.
	const uint20_t address = short_direct_address_to_absolute_address(saddr);
	return rl78core_mem_read_u08(address);
}

void rl78core_cpu_write_saddr(const uint8_t saddr, const uint8_t value)
{
	// note: do not need to assert the saddr bounds as there are 256 saddrs.
	const uint20_t address = short_direct_address_to_absolute_address(saddr);
	rl78core_mem_write_u08(address, value);
}

uint8_t rl78core_cpu_read_daddr(const uint8_t addrl, const uint8_t addrh)
{
	const uint20_t address = direct_address_to_absolute_address(addrl, addrh);
	return rl78core_mem_read_u08(address);
}

void rl78core_cpu_write_daddr(const uint8_t addrl, const uint8_t addrh, const uint8_t value)
{
	const uint20_t address = direct_address_to_absolute_address(addrl, addrh);
	rl78core_mem_write_u08(address, value);
}

#ifndef NDEBUG
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
#endif

void rl78core_cpu_tick(void)
{
	if (g_rl78core_cpu.halted)
	{
		return;
	} 
	
	// note: emulating the cycles ~~~v~~~~~~~~
	if (g_rl78core_cpu.instruction.cycles > 1)
	{
		--(g_rl78core_cpu.instruction.cycles);
		return;
	}

	// 1. MEM (memory access): Decoded instruction is executed and memory at target address is accessed.
	if (g_rl78core_cpu.halted)
	{
		return;
	}

	execute_instruction();

	// 2, ID (instruction decode): Instruction is decoded and address is calculated.
	if (g_rl78core_cpu.halted)
	{
		return;
	}

	decode_instruction();

	// 3. IF (instruction fetch): Instruction is fetched and fetch pointer is incremented.
	if (g_rl78core_cpu.halted)
	{
		return;
	}

	fetch_instruction();
}

static uint20_t short_direct_address_to_absolute_address(const uint8_t address)
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

static uint20_t direct_address_to_absolute_address(const uint8_t address_low, const uint8_t address_high)
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

static void fetch_instruction(void)
{
	g_rl78core_cpu.fetch.fetched = false;

	for (uint8_t index = 0; index < rl78core_ins_max_size; ++index)
	{
		// todo: handle the out of bounds event - reset?
		g_rl78core_cpu.fetch.buffer[index] = rl78core_mem_read_u08(g_rl78core_cpu.pc++);
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
	const uint8_t third_byte  = g_rl78core_cpu.fetch.buffer[2];
	const uint8_t fourth_byte = g_rl78core_cpu.fetch.buffer[3];

	switch (first_byte)
	{
		case 0x50:  // MOV X, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode = first_byte,
				.data   = second_byte,
				.length = 2,
				.cycles = 1,
			};
		} break;

		case 0x51:  // MOV A, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode = first_byte,
				.data   = second_byte,
				.length = 2,
				.cycles = 1,
			};
		} break;

		case 0x52:  // MOV C, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode = first_byte,
				.data   = second_byte,
				.length = 2,
				.cycles = 1,
			};
		} break;

		case 0x53:  // MOV B, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode = first_byte,
				.data   = second_byte,
				.length = 2,
				.cycles = 1,
			};
		} break;

		case 0x54:  // MOV E, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode = first_byte,
				.data   = second_byte,
				.length = 2,
				.cycles = 1,
			};
		} break;

		case 0x55:  // MOV D, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode = first_byte,
				.data   = second_byte,
				.length = 2,
				.cycles = 1,
			};
		} break;

		case 0x56:  // MOV L, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode = first_byte,
				.data   = second_byte,
				.length = 2,
				.cycles = 1,
			};
		} break;

		case 0x57:  // MOV H, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode = first_byte,
				.data   = second_byte,
				.length = 2,
				.cycles = 1,
			};
		} break;

		case 0xCD:  // MOV saddr, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = short_direct_address_to_absolute_address(second_byte),
				.data    = third_byte,
				.length  = 3,
				.cycles  = 1,
			};
		} break;

		case 0xCE:  // MOV sfr, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = special_function_register_to_absolute_address(second_byte),
				.data    = third_byte,
				.length  = 3,
				.cycles  = 1,
			};
		} break;

		case 0xCF:  // MOV !addr16, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = direct_address_to_absolute_address(second_byte, third_byte),
				.data    = fourth_byte,
				.length  = 4,
				.cycles  = 1,
			};
		} break;

		case 0x60:  // MOV A, X
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x62:  // MOV A, C
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x63:  // MOV A, B
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x64:  // MOV A, E
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x65:  // MOV A, D
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x66:  // MOV A, L
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x67:  // MOV A, H
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x70:  // MOV X, A
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x72:  // MOV C, A
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x73:  // MOV B, A
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x74:  // MOV E, A
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x75:  // MOV D, A
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x76:  // MOV L, A
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x77:  // MOV H, A
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.length  = 1,
				.cycles  = 1,
			};
		} break;

		case 0x8D:  // MOV A, saddr
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = short_direct_address_to_absolute_address(second_byte),
				.length  = 2,
				.cycles  = 1,
			};
		} break;

		case 0x9D:  // MOV saddr, A
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = short_direct_address_to_absolute_address(second_byte),
				.length  = 2,
				.cycles  = 1,
			};
		} break;

		case 0x8E:  // MOV A, sfr
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = special_function_register_to_absolute_address(second_byte),
				.length  = 2,
				.cycles  = 1,
			};
		} break;

		case 0x9E:  // MOV sfr, A
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = special_function_register_to_absolute_address(second_byte),
				.length  = 2,
				.cycles  = 1,
			};
		} break;

		case 0x41:  // MOV ES, #byte
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.data = second_byte,
				.length  = 2,
				.cycles  = 1,  // todo: set the actual value!
			};
		} break;

		case 0x8F:  // MOV A, !addr16
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = direct_address_to_absolute_address(second_byte, third_byte),
				.length  = 3,
				.cycles  = 1,  // todo: set the actual value!
			};
		} break;

		case 0x9F:  // MOV !addr16, A
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode  = first_byte,
				.address = direct_address_to_absolute_address(second_byte, third_byte),
				.length  = 3,
				.cycles  = 1,  // todo: set the actual value!
			};
		} break;

		case 0x00:  // NOP
		{
			g_rl78core_cpu.instruction = (const rl78core_ins_s)
			{
				.opcode = first_byte,
				.length = 1,
				.cycles = 1,
			};
		} break;

		case 0x61:  // MOV ES, saddr/HALT/STOP
		{
			switch (second_byte)
			{
				case 0xB8:  // MOV ES, saddr
				{
					g_rl78core_cpu.instruction = (const rl78core_ins_s)
					{
						.opcode = (uint16_t)(first_byte | (uint16_t)(second_byte << 8)),
						.address = short_direct_address_to_absolute_address(third_byte),
						.length = 3,
						.cycles = 1,
					};
				} break;

				case 0xED:  // HALT
				{
					g_rl78core_cpu.instruction = (const rl78core_ins_s)
					{
						.opcode = (uint16_t)(first_byte | (uint16_t)(second_byte << 8)),
						.length = 2,
						.cycles = 3,
					};
				} break;

				case 0xFD:  // STOP
				{
					g_rl78core_cpu.instruction = (const rl78core_ins_s)
					{
						.opcode = (uint16_t)(first_byte | (uint16_t)(second_byte << 8)),
						.length = 2,
						.cycles = 3,
					};
				} break;

				default:
				{
					rl78core_cpu_reset();
					return;
				} break;
			}
		} break;

		default:
		{
			rl78core_cpu_reset();
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
		case 0x50:  // MOV X, #byte
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_x, g_rl78core_cpu.instruction.data);
		} break;

		case 0x51:  // MOV A, #byte
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_a, g_rl78core_cpu.instruction.data);
		} break;

		case 0x52:  // MOV C, #byte
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_c, g_rl78core_cpu.instruction.data);
		} break;

		case 0x53:  // MOV B, #byte
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_b, g_rl78core_cpu.instruction.data);
		} break;

		case 0x54:  // MOV E, #byte
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_e, g_rl78core_cpu.instruction.data);
		} break;

		case 0x55:  // MOV D, #byte
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_d, g_rl78core_cpu.instruction.data);
		} break;

		case 0x56:  // MOV L, #byte
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_l, g_rl78core_cpu.instruction.data);
		} break;

		case 0x57:  // MOV H, #byte
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_h, g_rl78core_cpu.instruction.data);
		} break;

		case 0xCD:  // MOV saddr, #byte
		{
			rl78core_mem_write_u08(g_rl78core_cpu.instruction.address, g_rl78core_cpu.instruction.data);
		} break;

		case 0xCE:  // MOV sfr, #byte
		{
			rl78core_mem_write_u08(g_rl78core_cpu.instruction.address, g_rl78core_cpu.instruction.data);
		} break;

		case 0xCF:  // MOV !addr16, #byte
		{
			rl78core_mem_write_u08(g_rl78core_cpu.instruction.address, g_rl78core_cpu.instruction.data);
		} break;

		case 0x60:  // MOV A, X
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_a, rl78core_cpu_read_gpr08(rl78core_gpr08_x));
		} break;

		case 0x62:  // MOV A, C
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_a, rl78core_cpu_read_gpr08(rl78core_gpr08_c));
		} break;

		case 0x63:  // MOV A, B
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_a, rl78core_cpu_read_gpr08(rl78core_gpr08_b));
		} break;

		case 0x64:  // MOV A, E
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_a, rl78core_cpu_read_gpr08(rl78core_gpr08_e));
		} break;

		case 0x65:  // MOV A, D
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_a, rl78core_cpu_read_gpr08(rl78core_gpr08_d));
		} break;

		case 0x66:  // MOV A, L
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_a, rl78core_cpu_read_gpr08(rl78core_gpr08_l));
		} break;

		case 0x67:  // MOV A, H
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_a, rl78core_cpu_read_gpr08(rl78core_gpr08_h));
		} break;

		case 0x70:  // MOV X, A
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_x, rl78core_cpu_read_gpr08(rl78core_gpr08_a));
		} break;

		case 0x72:  // MOV C, A
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_c, rl78core_cpu_read_gpr08(rl78core_gpr08_a));
		} break;

		case 0x73:  // MOV B, A
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_b, rl78core_cpu_read_gpr08(rl78core_gpr08_a));
		} break;

		case 0x74:  // MOV E, A
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_e, rl78core_cpu_read_gpr08(rl78core_gpr08_a));
		} break;

		case 0x75:  // MOV D, A
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_d, rl78core_cpu_read_gpr08(rl78core_gpr08_a));
		} break;

		case 0x76:  // MOV L, A
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_l, rl78core_cpu_read_gpr08(rl78core_gpr08_a));
		} break;

		case 0x77:  // MOV H, A
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_h, rl78core_cpu_read_gpr08(rl78core_gpr08_a));
		} break;

		case 0x8D:  // MOV A, saddr
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_a, rl78core_mem_read_u08(g_rl78core_cpu.instruction.address));
		} break;

		case 0x9D:  // MOV saddr, A
		{
			rl78core_mem_write_u08(g_rl78core_cpu.instruction.address, rl78core_cpu_read_gpr08(rl78core_gpr08_a));
		} break;

		case 0x8E:  // MOV A, sfr
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_a, rl78core_mem_read_u08(g_rl78core_cpu.instruction.address));
		} break;

		case 0x9E:  // MOV sfr, A
		{
			rl78core_mem_write_u08(g_rl78core_cpu.instruction.address, rl78core_cpu_read_gpr08(rl78core_gpr08_a));
		} break;

		case 0x41:  // MOV ES, #byte
		{
			rl78core_cpu_write_sfr08(rl78core_sfr08_es, g_rl78core_cpu.instruction.data);
		} break;

		case 0xB861:  // MOV ES, saddr
		{
			rl78core_cpu_write_sfr08(rl78core_sfr08_es, rl78core_mem_read_u08(g_rl78core_cpu.instruction.address));
		} break;

		case 0x8F:  // MOV A, !addr16
		{
			rl78core_cpu_write_gpr08(rl78core_gpr08_a, rl78core_mem_read_u08(g_rl78core_cpu.instruction.address));
		} break;

		case 0x9F:  // MOV !addr16, A
		{
			rl78core_mem_write_u08(g_rl78core_cpu.instruction.address, rl78core_cpu_read_gpr08(rl78core_gpr08_a));
		} break;

		case 0x00:  // NOP
		{
		} break;

		case 0xED61:  // HALT
		{
			rl78core_cpu_halt();
		} break;

		case 0xFD61:  // STOP
		{
			rl78misc_debug_assert(!"0xFD61 (STOP) is not implemented yet!");
			// todo: implement!
		} break;

		default:
		{
			rl78core_cpu_reset();
			return;
		} break;
	}
}
