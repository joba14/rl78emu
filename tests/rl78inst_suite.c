
/**
 * @file rl78misc_suite.c
 * 
 * @copyright This file is a part of the "rl78emu" project and is licensed, and
 * distributed under "rl78emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-07
 */

#include "rl78core/mem.h"
#include "rl78core/cpu.h"

#include "./utester.h"

#define vargs(...)                                                             \
	{                                                                          \
		__VA_ARGS__                                                            \
	}

#define init_env_flash_rom_and_display_info(_info, _rom)                       \
	do                                                                         \
	{                                                                          \
		rl78core_mem_init();                                                   \
		rl78core_cpu_init();                                                   \
		                                                                       \
		const uint8_t rom[] = _rom;                                            \
		const uint64_t rom_length = (sizeof(rom) / sizeof(rom[0]));            \
		                                                                       \
		for (uint20_t address = 0; address < rom_length; ++address)            \
		{                                                                      \
			rl78core_mem_write_u08(address, rom[address]);                     \
		}                                                                      \
		                                                                       \
		const char_t* lines[] = _info;                                         \
		const uint64_t lines_count = (sizeof(lines) / sizeof(lines[0]));       \
		                                                                       \
		for (uint64_t index = 0; index < lines_count; ++index)                 \
		{                                                                      \
			utester_logger_info("    %s", lines[index]);                       \
		}                                                                      \
		                                                                       \
		utester_logger_info(                                                   \
			"    * Reference - "                                               \
			"https://llvm-gcc-renesas.com/pdf/r01us0015ej0220_rl78.pdf"        \
		);                                                                     \
		                                                                       \
		utester_logger_info(                                                   \
			"    * Bytes     - "                                               \
			"Length of the entire instruction in bytes."                       \
		);                                                                     \
		                                                                       \
		utester_logger_info(                                                   \
			"    * Clocks 1  - "                                               \
			"Number of CPU clocks (f CLK ) when the internal RAM area, SFR "   \
			"area, or extended SFR area is accessed, or when no data is "      \
			"accessed."                                                        \
		);                                                                     \
		                                                                       \
		utester_logger_info(                                                   \
			"    * Clocks 2  - "                                               \
			"Number of CPU clocks (fCLK) when the code flash memory area "     \
			"is accessed."                                                     \
			"Number of CPU clocks (f CLK ) when the code flash memory area "   \
			"is accessed, or when the data flash memory area is accessed by "  \
			"an 8-bit instruction."                                            \
		);                                                                     \
	} while (0)

#define tick_n_times(_n)                                                       \
	do                                                                         \
	{                                                                          \
		for (uint64_t index = 0; index < (_n); ++index)                        \
		{                                                                      \
			rl78core_cpu_tick();                                               \
		}                                                                      \
	} while (0)

typedef struct
{
	uint8_t x, a, c, b, e, d, l, h;
	uint16_t ax, bc, de, hl;
	uint8_t spl, sph, psw, cs, es, pmc, mem;
} rl78_cpu_state_s;

#define rl78_cpu_state_fetch()                                                 \
	(rl78_cpu_state_s)                                                         \
	{                                                                          \
		.x   = rl78core_mem_read_u08(                                          \
			rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_x)           \
		),                                                                     \
		.a   = rl78core_mem_read_u08(                                          \
			rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_a)           \
		),                                                                     \
		.c   = rl78core_mem_read_u08(                                          \
			rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_c)           \
		),                                                                     \
		.b   = rl78core_mem_read_u08(                                          \
			rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_b)           \
		),                                                                     \
		.e   = rl78core_mem_read_u08(                                          \
			rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_e)           \
		),                                                                     \
		.d   = rl78core_mem_read_u08(                                          \
			rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_d)           \
		),                                                                     \
		.l   = rl78core_mem_read_u08(                                          \
			rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_l)           \
		),                                                                     \
		.h   = rl78core_mem_read_u08(                                          \
			rl78core_cpu_gpr08_to_absolute_address(rl78core_gpr08_h)           \
		),                                                                     \
		.ax  = rl78core_mem_read_u16(                                          \
			rl78core_cpu_gpr16_to_absolute_address(rl78core_gpr16_ax)          \
		),                                                                     \
		.bc  = rl78core_mem_read_u16(                                          \
			rl78core_cpu_gpr16_to_absolute_address(rl78core_gpr16_bc)          \
		),                                                                     \
		.de  = rl78core_mem_read_u16(                                          \
			rl78core_cpu_gpr16_to_absolute_address(rl78core_gpr16_de)          \
		),                                                                     \
		.hl  = rl78core_mem_read_u16(                                          \
			rl78core_cpu_gpr16_to_absolute_address(rl78core_gpr16_hl)          \
		),                                                                     \
		.spl = rl78core_mem_read_u08(                                          \
			rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_spl)         \
		),                                                                     \
		.sph = rl78core_mem_read_u08(                                          \
			rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_sph)         \
		),                                                                     \
		.psw = rl78core_mem_read_u08(                                          \
			rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_psw)         \
		),                                                                     \
		.cs  = rl78core_mem_read_u08(                                          \
			rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_cs)          \
		),                                                                     \
		.es  = rl78core_mem_read_u08(                                          \
			rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_es)          \
		),                                                                     \
		.pmc = rl78core_mem_read_u08(                                          \
			rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_pmc)         \
		),                                                                     \
		.mem = rl78core_mem_read_u08(                                          \
			rl78core_cpu_sfr08_to_absolute_address(rl78core_sfr08_mem)         \
		),                                                                     \
	}

#define assert_rl78_cpu_state_equal(_left_state, _right_state)                 \
	do                                                                         \
	{                                                                          \
		utester_assert_equal((_left_state).x  , (_right_state).x  );           \
		utester_assert_equal((_left_state).a  , (_right_state).a  );           \
		utester_assert_equal((_left_state).c  , (_right_state).c  );           \
		utester_assert_equal((_left_state).b  , (_right_state).b  );           \
		utester_assert_equal((_left_state).e  , (_right_state).e  );           \
		utester_assert_equal((_left_state).d  , (_right_state).d  );           \
		utester_assert_equal((_left_state).l  , (_right_state).l  );           \
		utester_assert_equal((_left_state).h  , (_right_state).h  );           \
		utester_assert_equal((_left_state).ax , (_right_state).ax );           \
		utester_assert_equal((_left_state).bc , (_right_state).bc );           \
		utester_assert_equal((_left_state).de , (_right_state).de );           \
		utester_assert_equal((_left_state).hl , (_right_state).hl );           \
		utester_assert_equal((_left_state).spl, (_right_state).spl);           \
		utester_assert_equal((_left_state).sph, (_right_state).sph);           \
		utester_assert_equal((_left_state).psw, (_right_state).psw);           \
		utester_assert_equal((_left_state).cs , (_right_state).cs );           \
		utester_assert_equal((_left_state).es , (_right_state).es );           \
		utester_assert_equal((_left_state).pmc, (_right_state).pmc);           \
		utester_assert_equal((_left_state).mem, (_right_state).mem);           \
	} while (0)

utester_define_test(rl78inst_move_x_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+----------------+-------------+-------+----------+----------+",
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |",
			"+----------------+-------------+-------+----------+----------+",
			"|  MOV X, #byte  |  r <- byte  |   2   |    1     |    -     |",
			"+----------------+-------------+-------+----------+----------+",
		),
		vargs(
			0x50, 0x0A,  // MOV X, 0x0A
			0x50, 0x00,  // MOV X, 0x00
			0x50, 0x0A,  // MOV X, 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.x = 0x0A; expected_state.ax = 0x000A;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.x = 0x00; expected_state.ax = 0x0000;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.x = 0x0A; expected_state.ax = 0x000A;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_a_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+----------------+-------------+-------+----------+----------+",
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |",
			"+----------------+-------------+-------+----------+----------+",
			"|  MOV A, #byte  |  r <- byte  |   2   |    1     |    -     |",
			"+----------------+-------------+-------+----------+----------+",
		),
		vargs(
			0x51, 0x0A,  // MOV A, 0x0A
			0x51, 0x00,  // MOV A, 0x00
			0x51, 0x0A,  // MOV A, 0x0A
		)
	);


	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.a = 0x0A; expected_state.ax = 0x0A00;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.a = 0x00; expected_state.ax = 0x0000;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.a = 0x0A; expected_state.ax = 0x0A00;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_c_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+----------------+-------------+-------+----------+----------+",
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |",
			"+----------------+-------------+-------+----------+----------+",
			"|  MOV C, #byte  |  r <- byte  |   2   |    1     |    -     |",
			"+----------------+-------------+-------+----------+----------+",
		),
		vargs(
			0x52, 0x0A,  // MOV C, 0x0A
			0x52, 0x00,  // MOV C, 0x00
			0x52, 0x0A,  // MOV C, 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.c = 0x0A; expected_state.bc = 0x000A;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.c = 0x00; expected_state.bc = 0x0000;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.c = 0x0A; expected_state.bc = 0x000A;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_b_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+----------------+-------------+-------+----------+----------+",
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |",
			"+----------------+-------------+-------+----------+----------+",
			"|  MOV B, #byte  |  r <- byte  |   2   |    1     |    -     |",
			"+----------------+-------------+-------+----------+----------+",
		),
		vargs(
			0x53, 0x0A,  // MOV B, 0x0A
			0x53, 0x00,  // MOV B, 0x00
			0x53, 0x0A,  // MOV B, 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.b = 0x0A; expected_state.bc = 0x0A00;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.b = 0x00; expected_state.bc = 0x0000;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.b = 0x0A; expected_state.bc = 0x0A00;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_e_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+----------------+-------------+-------+----------+----------+",
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |",
			"+----------------+-------------+-------+----------+----------+",
			"|  MOV E, #byte  |  r <- byte  |   2   |    1     |    -     |",
			"+----------------+-------------+-------+----------+----------+",
		),
		vargs(
			0x54, 0x0A,  // MOV E, 0x0A
			0x54, 0x00,  // MOV E, 0x00
			0x54, 0x0A,  // MOV E, 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.e = 0x0A; expected_state.de = 0x000A;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.e = 0x00; expected_state.de = 0x0000;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.e = 0x0A; expected_state.de = 0x000A;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_d_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+----------------+-------------+-------+----------+----------+",
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |",
			"+----------------+-------------+-------+----------+----------+",
			"|  MOV D, #byte  |  r <- byte  |   2   |    1     |    -     |",
			"+----------------+-------------+-------+----------+----------+",
		),
		vargs(
			0x55, 0x0A,  // MOV D, 0x0A
			0x55, 0x00,  // MOV D, 0x00
			0x55, 0x0A,  // MOV D, 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.d = 0x0A; expected_state.de = 0x0A00;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.d = 0x00; expected_state.de = 0x0000;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.d = 0x0A; expected_state.de = 0x0A00;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_l_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+----------------+-------------+-------+----------+----------+",
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |",
			"+----------------+-------------+-------+----------+----------+",
			"|  MOV L, #byte  |  r <- byte  |   2   |    1     |    -     |",
			"+----------------+-------------+-------+----------+----------+",
		),
		vargs(
			0x56, 0x0A,  // MOV L, 0x0A
			0x56, 0x00,  // MOV L, 0x00
			0x56, 0x0A,  // MOV L, 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.l = 0x0A; expected_state.hl = 0x000A;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.l = 0x00; expected_state.hl = 0x0000;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.l = 0x0A; expected_state.hl = 0x000A;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_h_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+----------------+-------------+-------+----------+----------+",
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |",
			"+----------------+-------------+-------+----------+----------+",
			"|  MOV H, #byte  |  r <- byte  |   2   |    1     |    -     |",
			"+----------------+-------------+-------+----------+----------+",
		),
		vargs(
			0x57, 0x0A,  // MOV H, 0x0A
			0x57, 0x00,  // MOV H, 0x00
			0x57, 0x0A,  // MOV H, 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.h = 0x0A; expected_state.hl = 0x0A00;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.h = 0x00; expected_state.hl = 0x0000;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.h = 0x0A; expected_state.hl = 0x0A00;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_psw_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+------------------+---------------+-------+----------+----------+",
			"| Instruction      | Template      | Bytes | Clocks 1 | Clocks 2 |",
			"+------------------+---------------+-------+----------+----------+",
			"|  MOV PSW, #byte  |  PSW <- byte  |   3   |    3     |    -     |",
			"+------------------+---------------+-------+----------+----------+",
		),
		vargs(
			0xCE, 0xFA, 0x0A,  // MOV PSW, 0x0A
			0xCE, 0xFA, 0x00,  // MOV PSW, 0x00
			0xCE, 0xFA, 0x0A,  // MOV PSW, 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.psw = 0x0A;
	tick_n_times(3); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.psw = 0x00;
	tick_n_times(3); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.psw = 0x0A;
	tick_n_times(3); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_cs_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+-----------------+--------------+-------+----------+----------+",
			"| Instruction     | Template     | Bytes | Clocks 1 | Clocks 2 |",
			"+-----------------+--------------+-------+----------+----------+",
			"|  MOV CS, #byte  |  CS <- byte  |   3   |    1     |    -     |",
			"+-----------------+--------------+-------+----------+----------+",
		),
		vargs(
			0xCE, 0xFC, 0x0A,  // MOV CS, 0x0A
			0xCE, 0xFC, 0x00,  // MOV CS, 0x00
			0xCE, 0xFC, 0x0A,  // MOV CS, 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.cs = 0x0A;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.cs = 0x00;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.cs = 0x0A;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_es_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+-----------------+--------------+-------+----------+----------+",
			"| Instruction     | Template     | Bytes | Clocks 1 | Clocks 2 |",
			"+-----------------+--------------+-------+----------+----------+",
			"|  MOV ES, #byte  |  ES <- byte  |   2   |    1     |    -     |",
			"+-----------------+--------------+-------+----------+----------+",
		),
		vargs(
			0x41, 0x0A,  // MOV ES, 0x0A
			0x41, 0x00,  // MOV ES, 0x00
			0x41, 0x0A,  // MOV ES, 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.es = 0x0A;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.es = 0x00;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	expected_state.es = 0x0A;
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_addr16_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+----------------------+--------------------+-------+----------+----------+",
			"| Instruction          | Template           | Bytes | Clocks 1 | Clocks 2 |",
			"+----------------------+--------------------+-------+----------+----------+",
			"|  MOV !addr16, #byte  |  (addr16) <- byte  |   4   |    1     |    -     |",
			"+----------------------+--------------------+-------+----------+----------+",
		),
		vargs(
			0xCF, 0x0A, 0xF0, 0x0A,  // MOV addr16(0x'F0'0A), 0x0A
			0xCF, 0x0A, 0xF0, 0x00,  // MOV addr16(0x'F0'0A), 0x00
			0xCF, 0x0A, 0xF0, 0x0A,  // MOV addr16(0x'F0'0A), 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	utester_assert_equal(rl78core_mem_read_u08(0xFF00A), 0x0A);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	utester_assert_equal(rl78core_mem_read_u08(0xFF00A), 0x00);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	utester_assert_equal(rl78core_mem_read_u08(0xFF00A), 0x0A);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_es_addr16_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+-------------------------+------------------------+-------+----------+----------+",
			"| Instruction             | Template               | Bytes | Clocks 1 | Clocks 2 |",
			"+-------------------------+------------------------+-------+----------+----------+",
			"|  MOV ES:!addr16, #byte  |  (ES, addr16) <- byte  |   5   |    2     |    -     |",
			"+-------------------------+------------------------+-------+----------+----------+",
		),
		vargs(
			0x11, 0xCF, 0x0A, 0xF0, 0x0A,  // MOV ES:addr16(0x'F0'0A), 0x0A
			0x11, 0xCF, 0x0A, 0xF0, 0x00,  // MOV ES:addr16(0x'F0'0A), 0x00
			0x11, 0xCF, 0x0A, 0xF0, 0x0A,  // MOV ES:addr16(0x'F0'0A), 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(2); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	utester_assert_equal(rl78core_mem_read_u08(0x0F00A), 0x0A);

	tick_n_times(2); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	utester_assert_equal(rl78core_mem_read_u08(0x0F00A), 0x00);

	tick_n_times(2); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	utester_assert_equal(rl78core_mem_read_u08(0x0F00A), 0x0A);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_saddr_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+--------------------+-------------------+-------+----------+----------+",
			"| Instruction        | Template          | Bytes | Clocks 1 | Clocks 2 |",
			"+--------------------+-------------------+-------+----------+----------+",
			"|  MOV saddr, #byte  |  (saddr) <- byte  |   3   |    1     |    -     |",
			"+--------------------+-------------------+-------+----------+----------+",
		),
		vargs(
			0xCD, 0x0A, 0x0A,  // MOV saddr(0x'0A), 0x0A
			0xCD, 0x0A, 0x00,  // MOV saddr(0x'0A), 0x00
			0xCD, 0x0A, 0x0A,  // MOV saddr(0x'0A), 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	utester_assert_equal(rl78core_mem_read_u08(0xFFE20 + 0x0A), 0x0A);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	utester_assert_equal(rl78core_mem_read_u08(0xFFE20 + 0x0A), 0x00);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	utester_assert_equal(rl78core_mem_read_u08(0xFFE20 + 0x0A), 0x0A);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_define_test(rl78inst_move_sfr_byte_test)
{
	init_env_flash_rom_and_display_info(
		vargs(
			"+------------------+---------------+-------+----------+----------+",
			"| Instruction      | Template      | Bytes | Clocks 1 | Clocks 2 |",
			"+------------------+---------------+-------+----------+----------+",
			"|  MOV sfr, #byte  |  sfr <- byte  |   3   |    1     |    -     |",
			"+------------------+---------------+-------+----------+----------+",
		),
		vargs(
			0xCE, 0x0A, 0x0A,  // MOV sfr(0x'0A), 0x0A
			0xCE, 0x0A, 0x00,  // MOV sfr(0x'0A), 0x00
			0xCE, 0x0A, 0x0A,  // MOV sfr(0x'0A), 0x0A
		)
	);

	rl78_cpu_state_s actual_state   = (rl78_cpu_state_s) {0};
	rl78_cpu_state_s expected_state = (rl78_cpu_state_s) {0};

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	utester_assert_equal(rl78core_mem_read_u08(rl78core_cpu_sfr08_to_absolute_address(0x0A)), 0x0A);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	utester_assert_equal(rl78core_mem_read_u08(rl78core_cpu_sfr08_to_absolute_address(0x0A)), 0x00);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	utester_assert_equal(rl78core_mem_read_u08(rl78core_cpu_sfr08_to_absolute_address(0x0A)), 0x0A);

	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
	tick_n_times(1); actual_state = rl78_cpu_state_fetch(); assert_rl78_cpu_state_equal(actual_state, expected_state);
}

utester_run_suite(
	rl78inst_suite,
		&rl78inst_move_x_byte_test,
		&rl78inst_move_a_byte_test,
		&rl78inst_move_c_byte_test,
		&rl78inst_move_b_byte_test,
		&rl78inst_move_e_byte_test,
		&rl78inst_move_d_byte_test,
		&rl78inst_move_l_byte_test,
		&rl78inst_move_h_byte_test,
		&rl78inst_move_psw_byte_test,
		&rl78inst_move_cs_byte_test,
		&rl78inst_move_es_byte_test,
		&rl78inst_move_addr16_byte_test,
		&rl78inst_move_es_addr16_byte_test,
		&rl78inst_move_saddr_byte_test,
		&rl78inst_move_sfr_byte_test,
);
