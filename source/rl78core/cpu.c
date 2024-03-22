
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

typedef struct
{
	#define rl78core_inst_max_size 5
	uint8_t buffer[rl78core_inst_max_size];
	bool_t  fetched;
} rl78core_fetch_s;

typedef enum
{
	rl78core_inst_mov,
	rl78core_inst_xch,
	rl78core_inst_oneb,
	rl78core_inst_clrb,
	rl78core_inst_movs,

	rl78core_inst_movw,
	rl78core_inst_xchw,
	rl78core_inst_onew,
	rl78core_inst_clrw,

	rl78core_inst_add,
	rl78core_inst_addc,
	rl78core_inst_sub,
	rl78core_inst_subc,
	rl78core_inst_and,
	rl78core_inst_or,
	rl78core_inst_xor,
	rl78core_inst_cmp,
	rl78core_inst_cmp0,
	rl78core_inst_cmps,

	rl78core_inst_addw,
	rl78core_inst_subw,
	rl78core_inst_cmpw,

	rl78core_inst_mulu,
	rl78core_inst_mulhu,
	rl78core_inst_mulh,
	rl78core_inst_divhu,
	rl78core_inst_divwu,
	rl78core_inst_machu,
	rl78core_inst_mach,

	rl78core_inst_inc,
	rl78core_inst_dec,
	rl78core_inst_incw,
	rl78core_inst_decw,

	rl78core_inst_shr,
	rl78core_inst_shrw,
	rl78core_inst_shl,
	rl78core_inst_shlw,
	rl78core_inst_sar,
	rl78core_inst_sarw,

	rl78core_inst_ror,
	rl78core_inst_rol,
	rl78core_inst_rorc,
	rl78core_inst_rolc,
	rl78core_inst_rolwc,

	rl78core_inst_mov1,
	rl78core_inst_and1,
	rl78core_inst_or1,
	rl78core_inst_xor1,
	rl78core_inst_set1,
	rl78core_inst_clr1,
	rl78core_inst_not1,

	rl78core_inst_call,
	rl78core_inst_callt,
	rl78core_inst_brk,
	rl78core_inst_ret,
	rl78core_inst_reti,
	rl78core_inst_retb,

	rl78core_inst_push,
	rl78core_inst_pop,
	rl78core_inst_movw_sp_src,
	rl78core_inst_movw_rp_sp,
	rl78core_inst_addw_sp_byte,
	rl78core_inst_subw_sp_byte,

	rl78core_inst_br,

	rl78core_inst_bc,
	rl78core_inst_bnc,
	rl78core_inst_bz,
	rl78core_inst_bnz,
	rl78core_inst_bh,
	rl78core_inst_bnh,
	rl78core_inst_bt,
	rl78core_inst_bf,
	rl78core_inst_btclr,

	rl78core_inst_skc,
	rl78core_inst_sknc,
	rl78core_inst_skz,
	rl78core_inst_sknz,
	rl78core_inst_skh,
	rl78core_inst_sknh,

	rl78core_inst_sel_rbn,
	rl78core_inst_nop,
	rl78core_inst_ei,
	rl78core_inst_di,
	rl78core_inst_halt,
	rl78core_inst_stop,
} rl78core_inst_e;

typedef struct
{
	rl78core_inst_e type;
	uint8_t cycles;
	uint8_t length;
	bool_t  decoded;

	union
	{
		struct
		{
			uint20_t address;
			uint8_t  data;
		} mov;
	};
} rl78core_inst_s;

#define inst_cycles(_amount) ((uint8_t)(_amount))
#define inst_length(_amount) ((uint8_t)(_amount))

#define rl78core_inst_create_mov(_cycles, _length, _address, _data)            \
	(const rl78core_inst_s)                                                    \
	{                                                                          \
		.type    = rl78core_inst_mov,                                          \
		.cycles  = (_cycles),                                                  \
		.length  = (_length),                                                  \
		.decoded = false,                                                      \
		.mov =                                                                 \
		{                                                                      \
			.address = (_address),                                             \
			.data    = (_data),                                                \
		}                                                                      \
	}

typedef struct
{
	rl78core_fetch_s fetch;
	rl78core_inst_s  inst;
	uint20_t         pc;
	bool_t           halted;
} rl78core_cpu_s;

static rl78core_cpu_s g_rl78core_cpu;

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
		.fetch  = (const rl78core_fetch_s) {0},
		.inst   = (const rl78core_inst_s)  {0},
		.pc     = 0x00000,
		.halted = false,
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

uint20_t rl78core_cpu_pc_read(void)
{
	return g_rl78core_cpu.pc;
}

void rl78core_cpu_pc_write(const uint20_t value)
{
	rl78misc_debug_assert(value < 0x000FFFFF);
	g_rl78core_cpu.pc = value;
}

uint20_t rl78core_cpu_gpr08_to_absolute_address(const uint8_t gpr08)
{
	rl78misc_debug_assert(gpr08 < rl78core_gprs_count);
	const uint8_t  general_purpose_register_count_per_bank = 8;
	const uint20_t general_purpose_register_start = 0xFFEE0;
	const uint8_t  general_purpose_register_length = 0x20;

	rl78misc_debug_assert(gpr08 < general_purpose_register_count_per_bank);
	const uint20_t psw_absolute_address = rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_psw);
	const uint8_t psw_value = rl78core_mem_read_u08(psw_absolute_address);
	// +----+---+------+----+------+------+------+----+      +------+------+
	// | IE | Z | RBS1 | AC | RBS0 | ISP1 | ISP0 | CY |  ->  | RBS1 | RBS0 |
	// +----+---+------+----+------+------+------+----+      +------+------+
	const uint8_t current_gpr_bank = (const uint8_t)(
		(const uint8_t)((const uint8_t)(psw_value & 0x08) >> 3) |
		(const uint8_t)((const uint8_t)(psw_value & 0x20) >> 4)
	);

	const uint8_t address = (const uint8_t)(
		(const uint8_t)(current_gpr_bank * general_purpose_register_count_per_bank) + gpr08
	);

	rl78misc_debug_assert((general_purpose_register_start + address) <
		(general_purpose_register_start + general_purpose_register_length)
	);

	const uint20_t absolute_address = general_purpose_register_start + address;
	return absolute_address;
}

uint20_t rl78core_cpu_gpr16_to_absolute_address(const uint8_t gpr16)
{
	rl78misc_debug_assert(0 == (gpr16 % 2));
	return rl78core_cpu_gpr08_to_absolute_address(gpr16);
}

uint20_t rl78core_cpu_sfr08_to_absolute_address(const uint8_t sfr08)
{
	// note: do not need to assert the sfr08 bounds as there are 256 sfrs.
	const uint20_t special_function_register_start = 0xFFF00;
	const uint16_t special_function_register_length = 0x100;

	rl78misc_debug_assert((special_function_register_start + sfr08) <
		(special_function_register_start + special_function_register_length)
	);

	const uint20_t absolute_address = special_function_register_start + sfr08;
	return absolute_address;
}

uint20_t rl78core_cpu_sfr16_to_absolute_address(const uint8_t sfr16)
{
	// note: do not need to assert the sfr08 bounds as there are 256 sfrs.
	rl78misc_debug_assert(0 == (sfr16 % 2));
	return rl78core_cpu_sfr08_to_absolute_address(sfr16);
}

void rl78core_cpu_tick(void)
{
	if (g_rl78core_cpu.halted) { return; }

	// note: emulating the cycles ~v~~~
	if (g_rl78core_cpu.inst.cycles > 1)
	{
		--(g_rl78core_cpu.inst.cycles);
		return;
	}

	if (g_rl78core_cpu.halted) { return; }
	execute_instruction();

	if (g_rl78core_cpu.halted) { return; }
	decode_instruction();

	if (g_rl78core_cpu.halted) { return; }
	fetch_instruction();
}

static void fetch_instruction(void)
{
	g_rl78core_cpu.fetch.fetched = false;

	for (uint8_t index = 0; index < rl78core_inst_max_size; ++index)
	{
		// todo: handle the out of bounds event - reset?
		const uint20_t pc = rl78core_cpu_pc_read();
		g_rl78core_cpu.fetch.buffer[index] = rl78core_mem_read_u08(pc);
		rl78core_cpu_pc_write(pc + 1);
	}

	g_rl78core_cpu.fetch.fetched = true;
}

static void decode_instruction(void)
{
	if (!g_rl78core_cpu.fetch.fetched)
	{
		g_rl78core_cpu.inst.decoded = false;
		return;
	}

	const uint8_t first_byte  = g_rl78core_cpu.fetch.buffer[0]; (void)first_byte;
	const uint8_t second_byte = g_rl78core_cpu.fetch.buffer[1]; (void)second_byte;
	const uint8_t third_byte  = g_rl78core_cpu.fetch.buffer[2]; (void)third_byte;
	const uint8_t fourth_byte = g_rl78core_cpu.fetch.buffer[3]; (void)fourth_byte;
	const uint8_t fifth_byte  = g_rl78core_cpu.fetch.buffer[4]; (void)fifth_byte;

	switch (first_byte)
	{
		case 0x50: { g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(1), inst_length(2), rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_x), second_byte); } break;  // MOV X, #byte
		case 0x51: { g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(1), inst_length(2), rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_a), second_byte); } break;  // MOV A, #byte
		case 0x52: { g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(1), inst_length(2), rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_c), second_byte); } break;  // MOV C, #byte
		case 0x53: { g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(1), inst_length(2), rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_b), second_byte); } break;  // MOV B, #byte
		case 0x54: { g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(1), inst_length(2), rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_e), second_byte); } break;  // MOV E, #byte
		case 0x55: { g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(1), inst_length(2), rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_d), second_byte); } break;  // MOV D, #byte
		case 0x56: { g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(1), inst_length(2), rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_l), second_byte); } break;  // MOV L, #byte
		case 0x57: { g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(1), inst_length(2), rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_h), second_byte); } break;  // MOV H, #byte

		case 0xCE:  // MOV PSW, #byte/MOV CS, #byte/MOV sfr, #byte
		{
			switch (second_byte)
			{
				case 0xFA: { g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(3), inst_length(3), rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_psw), third_byte); } break;  // MOV PSW, #byte
				case 0xFC: { g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(1), inst_length(3), rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_cs ), third_byte); } break;  // MOV CS,  #byte

				default:
				{
					const uint20_t absolute_address = rl78core_cpu_sfr08_to_absolute_address(second_byte);
					g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(1), inst_length(3), absolute_address, third_byte);
				} break;  // MOV sfr, #byte
			}
		} break;

		case 0x41: { g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(1), inst_length(2), rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_es), second_byte); } break;  // MOV ES, #byte

		case 0xCF:
		{
			const uint20_t absolute_address = (const uint20_t)(0xF0000 | (const uint20_t)(second_byte | (const uint20_t)(third_byte << 8)));
			g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(1), inst_length(4), absolute_address, fourth_byte);
		} break;  // MOV !addr16, #byte

		case 0x11:
		{
			switch (second_byte)
			{
				case 0xCF:
				{
					const uint8_t es = rl78core_mem_read_u08(rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_es));
					const uint20_t absolute_address = (const uint20_t)((const uint20_t)(es << 16) | (const uint20_t)(third_byte | (const uint20_t)(fourth_byte << 8)));
					g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(2), inst_length(5), absolute_address, fifth_byte);
				} break;  // MOV ES:!addr16, #byte

				default:
				{
					// note: uncomment?
					// rl78core_cpu_reset();
					// return;
					g_rl78core_cpu.inst.decoded = false;
					return;
				} break;
			}
		} break;  // MOV !addr16, #byte

		case 0xCD:
		{
			const uint20_t absolute_address = 0xFFE20 + (const uint20_t)(second_byte);
			g_rl78core_cpu.inst = rl78core_inst_create_mov(inst_cycles(1), inst_length(3), absolute_address, third_byte);
		} break;  // MOV saddr, #byte

		default:
		{
			// note: uncomment?
			// rl78core_cpu_reset();
			// return;
			g_rl78core_cpu.inst.decoded = false;
			return;
		} break;
	}

	const uint20_t pc = rl78core_cpu_pc_read();
	rl78misc_debug_assert(rl78core_inst_max_size >= g_rl78core_cpu.inst.length);
	const uint8_t offset = (const uint8_t)(rl78core_inst_max_size - g_rl78core_cpu.inst.length);
	rl78core_cpu_pc_write(pc - offset);
	g_rl78core_cpu.inst.decoded = true;
}

static void execute_instruction(void)
{
	if (!g_rl78core_cpu.inst.decoded)
	{
		return;
	}

	switch (g_rl78core_cpu.inst.type)
	{
		case rl78core_inst_mov: { rl78core_mem_write_u08(g_rl78core_cpu.inst.mov.address, g_rl78core_cpu.inst.mov.data); } break;

		default:
		{
			rl78core_cpu_reset();
			return;
		} break;
	}
}
