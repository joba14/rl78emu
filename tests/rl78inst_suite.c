
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

#define comma ,

#define tick_amount(_n)                                                        \
	do                                                                         \
	{                                                                          \
		for (uint64_t index = 0; index < (_n); ++index)                        \
		{                                                                      \
			rl78core_cpu_tick();                                               \
		}                                                                      \
	} while (0)

utester_define_test(rl78inst_move_x_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+----------------+-------------+-------+----------+----------+" comma
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |" comma
			"+----------------+-------------+-------+----------+----------+" comma
			"|  MOV X, #byte  |  r <- byte  |   2   |    1     |    -     |" comma
			"+----------------+-------------+-------+----------+----------+" comma
		}, {
			0x50 comma 0x0A comma  // MOV X, 0x0A
			0x50 comma 0x00 comma  // MOV X, 0x00
			0x50 comma 0x0A comma  // MOV X, 0x0A
		}
	);

	uint8_t x;
	tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x00);
	tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x00);
	tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x0A);
	tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x00);
	tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x0A);
	tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x0A);
	tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x0A);
}

utester_define_test(rl78inst_move_a_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+----------------+-------------+-------+----------+----------+" comma
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |" comma
			"+----------------+-------------+-------+----------+----------+" comma
			"|  MOV A, #byte  |  r <- byte  |   2   |    1     |    -     |" comma
			"+----------------+-------------+-------+----------+----------+" comma
		}, {
			0x51 comma 0x0A comma  // MOV A, 0x0A
			0x51 comma 0x00 comma  // MOV A, 0x00
			0x51 comma 0x0A comma  // MOV A, 0x0A
		}
	);


	uint8_t a;
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_c_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+----------------+-------------+-------+----------+----------+" comma
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |" comma
			"+----------------+-------------+-------+----------+----------+" comma
			"|  MOV C, #byte  |  r <- byte  |   2   |    1     |    -     |" comma
			"+----------------+-------------+-------+----------+----------+" comma
		}, {
			0x52 comma 0x0A comma  // MOV C, 0x0A
			0x52 comma 0x00 comma  // MOV C, 0x00
			0x52 comma 0x0A comma  // MOV C, 0x0A
		}
	);

	uint8_t c;
	tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x00);
	tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x00);
	tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x0A);
	tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x00);
	tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x0A);
	tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x0A);
	tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x0A);
}

utester_define_test(rl78inst_move_b_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+----------------+-------------+-------+----------+----------+" comma
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |" comma
			"+----------------+-------------+-------+----------+----------+" comma
			"|  MOV B, #byte  |  r <- byte  |   2   |    1     |    -     |" comma
			"+----------------+-------------+-------+----------+----------+" comma
		}, {
			0x53 comma 0x0A comma  // MOV B, 0x0A
			0x53 comma 0x00 comma  // MOV B, 0x00
			0x53 comma 0x0A comma  // MOV B, 0x0A
		}
	);

	uint8_t b;
	tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x00);
	tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x00);
	tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x0A);
	tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x00);
	tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x0A);
	tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x0A);
	tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x0A);
}

utester_define_test(rl78inst_move_e_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+----------------+-------------+-------+----------+----------+" comma
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |" comma
			"+----------------+-------------+-------+----------+----------+" comma
			"|  MOV E, #byte  |  r <- byte  |   2   |    1     |    -     |" comma
			"+----------------+-------------+-------+----------+----------+" comma
		}, {
			0x54 comma 0x0A comma  // MOV E, 0x0A
			0x54 comma 0x00 comma  // MOV E, 0x00
			0x54 comma 0x0A comma  // MOV E, 0x0A
		}
	);

	uint8_t e;
	tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x00);
	tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x00);
	tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x0A);
	tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x00);
	tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x0A);
	tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x0A);
	tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x0A);
}

utester_define_test(rl78inst_move_d_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+----------------+-------------+-------+----------+----------+" comma
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |" comma
			"+----------------+-------------+-------+----------+----------+" comma
			"|  MOV D, #byte  |  r <- byte  |   2   |    1     |    -     |" comma
			"+----------------+-------------+-------+----------+----------+" comma
		}, {
			0x55 comma 0x0A comma  // MOV D, 0x0A
			0x55 comma 0x00 comma  // MOV D, 0x00
			0x55 comma 0x0A comma  // MOV D, 0x0A
		}
	);

	uint8_t d;
	tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x00);
	tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x00);
	tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x0A);
	tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x00);
	tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x0A);
	tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x0A);
	tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x0A);
}

utester_define_test(rl78inst_move_l_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+----------------+-------------+-------+----------+----------+" comma
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |" comma
			"+----------------+-------------+-------+----------+----------+" comma
			"|  MOV L, #byte  |  r <- byte  |   2   |    1     |    -     |" comma
			"+----------------+-------------+-------+----------+----------+" comma
		}, {
			0x56 comma 0x0A comma  // MOV L, 0x0A
			0x56 comma 0x00 comma  // MOV L, 0x00
			0x56 comma 0x0A comma  // MOV L, 0x0A
		}
	);

	uint8_t l;
	tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x00);
	tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x00);
	tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x0A);
	tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x00);
	tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x0A);
	tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x0A);
	tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x0A);
}

utester_define_test(rl78inst_move_h_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+----------------+-------------+-------+----------+----------+" comma
			"| Instruction    | Template    | Bytes | Clocks 1 | Clocks 2 |" comma
			"+----------------+-------------+-------+----------+----------+" comma
			"|  MOV H, #byte  |  r <- byte  |   2   |    1     |    -     |" comma
			"+----------------+-------------+-------+----------+----------+" comma
		}, {
			0x57 comma 0x0A comma  // MOV H, 0x0A
			0x57 comma 0x00 comma  // MOV H, 0x00
			0x57 comma 0x0A comma  // MOV H, 0x0A
		}
	);

	uint8_t h;
	tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x00);
	tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x00);
	tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x0A);
	tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x00);
	tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x0A);
	tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x0A);
	tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x0A);
}

utester_define_test(rl78inst_move_saddr_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+--------------------+-------------------+-------+----------+----------+" comma
			"| Instruction        | Template          | Bytes | Clocks 1 | Clocks 2 |" comma
			"+--------------------+-------------------+-------+----------+----------+" comma
			"|  MOV saddr, #byte  |  (saddr) <- byte  |   3   |    1     |    -     |" comma
			"+--------------------+-------------------+-------+----------+----------+" comma
		}, {
			0xCD comma 0x0A comma 0x0A comma  // MOV saddr(0x0A), 0x0A
			0xCD comma 0x0A comma 0x00 comma  // MOV saddr(0x0A), 0x00
			0xCD comma 0x0A comma 0x0A comma  // MOV saddr(0x0A), 0x0A
		}
	);

	uint8_t value;
	tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x00);
	tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x00);
	tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x0A);
	tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x00);
	tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x0A);
	tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x0A);
	tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x0A);
}

utester_define_test(rl78inst_move_sfr_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+------------------+---------------+-------+----------+----------+" comma
			"| Instruction      | Template      | Bytes | Clocks 1 | Clocks 2 |" comma
			"+------------------+---------------+-------+----------+----------+" comma
			"|  MOV sfr, #byte  |  sfr <- byte  |   3   |    1     |    -     |" comma
			"+------------------+---------------+-------+----------+----------+" comma
		}, {
			0xCE comma 0xF8 comma 0x0A comma  // MOV sfr(0xF8), 0x0A
			0xCE comma 0xF8 comma 0x00 comma  // MOV sfr(0xF8), 0x00
			0xCE comma 0xF8 comma 0x0A comma  // MOV sfr(0xF8), 0x0A
		}
	);

	uint8_t spl;
	tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x00);
	tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x00);
	tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
	tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x00);
	tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
	tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
	tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
}

utester_define_test(rl78inst_move_addr16_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+----------------------+--------------------+-------+----------+----------+" comma
			"| Instruction          | Template           | Bytes | Clocks 1 | Clocks 2 |" comma
			"+----------------------+--------------------+-------+----------+----------+" comma
			"|  MOV !addr16, #byte  |  (addr16) <- byte  |   4   |    1     |    -     |" comma
			"+----------------------+--------------------+-------+----------+----------+" comma
		}, {
			0xCF comma 0xF0 comma 0x00 comma 0x0A comma  // MOV addr16(0xF0, 0x00), 0x0A
			0xCF comma 0xF0 comma 0x00 comma 0x00 comma  // MOV addr16(0xF0, 0x00), 0x00
			0xCF comma 0xF0 comma 0x00 comma 0x0A comma  // MOV addr16(0xF0, 0x00), 0x0A
		}
	);

	uint8_t value;
	tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x00);
	tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x00);
	tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x0A);
	tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x00);
	tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x0A);
	tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x0A);
	tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x0A);
}

utester_define_test(rl78inst_move_a_x_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV A, X   |  A <- r  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x60 comma  // MOV A, X
			0x60 comma  // MOV A, X
			0x60 comma  // MOV A, X
		}
	);

	uint8_t a;
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_x, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_x, 0x00); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_x, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_a_c_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV A, C   |  A <- r  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x62 comma  // MOV A, C
			0x62 comma  // MOV A, C
			0x62 comma  // MOV A, C
		}
	);

	uint8_t a;
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_c, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_c, 0x00); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_c, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_a_b_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV A, B   |  A <- r  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x63 comma  // MOV A, B
			0x63 comma  // MOV A, B
			0x63 comma  // MOV A, B
		}
	);

	uint8_t a;
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_b, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_b, 0x00); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_b, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_a_e_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV A, E   |  A <- r  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x64 comma  // MOV A, E
			0x64 comma  // MOV A, E
			0x64 comma  // MOV A, E
		}
	);

	uint8_t a;
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_e, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_e, 0x00); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_e, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_a_d_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV A, D   |  A <- r  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x65 comma  // MOV A, D
			0x65 comma  // MOV A, D
			0x65 comma  // MOV A, D
		}
	);

	uint8_t a;
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_d, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_d, 0x00); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_d, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_a_l_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV A, L   |  A <- r  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x66 comma  // MOV A, L
			0x66 comma  // MOV A, L
			0x66 comma  // MOV A, L
		}
	);

	uint8_t a;
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_l, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_l, 0x00); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_l, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_a_h_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV A, H   |  A <- r  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x67 comma  // MOV A, H
			0x67 comma  // MOV A, H
			0x67 comma  // MOV A, H
		}
	);

	uint8_t a;
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_h, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_h, 0x00); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_h, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_x_a_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV X, A   |  r <- A  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x70 comma  // MOV X, A
			0x70 comma  // MOV X, A
			0x70 comma  // MOV X, A
		}
	);

	uint8_t x;
	tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(x, 0x00);
	tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(x, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x0A);
	tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(x, 0x0A);
	tick_amount(1); x = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(x, 0x0A);
}

utester_define_test(rl78inst_move_c_a_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV C, A   |  r <- A  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x72 comma  // MOV C, A
			0x72 comma  // MOV C, A
			0x72 comma  // MOV C, A
		}
	);

	uint8_t c;
	tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(c, 0x00);
	tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(c, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x0A);
	tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(c, 0x0A);
	tick_amount(1); c = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(c, 0x0A);
}

utester_define_test(rl78inst_move_b_a_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV B, A   |  r <- A  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x73 comma  // MOV B, A
			0x73 comma  // MOV B, A
			0x73 comma  // MOV B, A
		}
	);

	uint8_t b;
	tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(b, 0x00);
	tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(b, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x0A);
	tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(b, 0x0A);
	tick_amount(1); b = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(b, 0x0A);
}

utester_define_test(rl78inst_move_e_a_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV E, A   |  r <- A  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x74 comma  // MOV E, A
			0x74 comma  // MOV E, A
			0x74 comma  // MOV E, A
		}
	);

	uint8_t e;
	tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(e, 0x00);
	tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(e, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x0A);
	tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(e, 0x0A);
	tick_amount(1); e = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(e, 0x0A);
}

utester_define_test(rl78inst_move_d_a_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV D, A   |  r <- A  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x75 comma  // MOV D, A
			0x75 comma  // MOV D, A
			0x75 comma  // MOV D, A
		}
	);

	uint8_t d;
	tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(d, 0x00);
	tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(d, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x0A);
	tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(d, 0x0A);
	tick_amount(1); d = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(d, 0x0A);
}

utester_define_test(rl78inst_move_l_a_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV L, A   |  r <- A  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x76 comma  // MOV L, A
			0x76 comma  // MOV L, A
			0x76 comma  // MOV L, A
		}
	);

	uint8_t l;
	tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(l, 0x00);
	tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(l, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x0A);
	tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(l, 0x0A);
	tick_amount(1); l = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(l, 0x0A);
}

utester_define_test(rl78inst_move_h_a_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+----------+-------+----------+----------+" comma
			"| Instruction | Template | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+----------+-------+----------+----------+" comma
			"|  MOV H, A   |  r <- A  |   1   |    1     |    -     |" comma
			"+-------------+----------+-------+----------+----------+" comma
		}, {
			0x77 comma  // MOV H, A
			0x77 comma  // MOV H, A
			0x77 comma  // MOV H, A
		}
	);

	uint8_t h;
	tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(h, 0x00);
	tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(h, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x0A);
	tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(h, 0x0A);
	tick_amount(1); h = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(h, 0x0A);
}

utester_define_test(rl78inst_move_a_saddr_test)
{
	init_env_flash_rom_and_display_info({
			"+----------------+----------------+-------+----------+----------+" comma
			"| Instruction    | Template       | Bytes | Clocks 1 | Clocks 2 |" comma
			"+----------------+----------------+-------+----------+----------+" comma
			"|  MOV A, saddr  |  A <- (saddr)  |   2   |    1     |    -     |" comma
			"+----------------+----------------+-------+----------+----------+" comma
		}, {
			0x8D comma 0x0A comma  // MOV A, saddr(0x0A)
			0x8D comma 0x0A comma  // MOV A, saddr(0x0A)
			0x8D comma 0x0A comma  // MOV A, saddr(0x0A)
		}
	);

	uint8_t a;
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_saddr(0x0A, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_saddr(0x0A, 0x00); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_saddr(0x0A, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_saddr_a_test)
{
	init_env_flash_rom_and_display_info({
			"+----------------+----------------+-------+----------+----------+" comma
			"| Instruction    | Template       | Bytes | Clocks 1 | Clocks 2 |" comma
			"+----------------+----------------+-------+----------+----------+" comma
			"|  MOV saddr, A  |  (saddr) <- A  |   2   |    1     |    -     |" comma
			"+----------------+----------------+-------+----------+----------+" comma
		}, {
			0x9D comma 0x0A comma  // MOV saddr(0x0A), A
			0x9D comma 0x0A comma  // MOV saddr(0x0A), A
			0x9D comma 0x0A comma  // MOV saddr(0x0A), A
		}
	);

	uint8_t value;
	tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x00);
	tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x0A);
	tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x0A);
	tick_amount(1); value = rl78core_cpu_read_saddr(0x0A); utester_assert_equal(value, 0x0A);
}

utester_define_test(rl78inst_move_a_sfr_test)
{
	init_env_flash_rom_and_display_info({
			"+--------------+------------+-------+----------+----------+" comma
			"| Instruction  | Template   | Bytes | Clocks 1 | Clocks 2 |" comma
			"+--------------+------------+-------+----------+----------+" comma
			"|  MOV sfr, A  |  A <- sfr  |   2   |    1     |    -     |" comma
			"+--------------+------------+-------+----------+----------+" comma
		}, {
			0x8E comma 0xF8 comma  // MOV A, sfr(0xF8)
			0x8E comma 0xF8 comma  // MOV A, sfr(0xF8)
			0x8E comma 0xF8 comma  // MOV A, sfr(0xF8)
		}
	);

	uint8_t a;
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_spl, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_sfr08(rl78core_sfr08_spl, 0x00); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_spl, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_sfr_a_test)
{
	init_env_flash_rom_and_display_info({
			"+--------------+------------+-------+----------+----------+" comma
			"| Instruction  | Template   | Bytes | Clocks 1 | Clocks 2 |" comma
			"+--------------+------------+-------+----------+----------+" comma
			"|  MOV A, sfr  |  sfr <- A  |   2   |    1     |    -     |" comma
			"+--------------+------------+-------+----------+----------+" comma
		}, {
			0x9E comma 0xF8 comma  // MOV sfr(0xF8), A
			0x9E comma 0xF8 comma  // MOV sfr(0xF8), A
			0x9E comma 0xF8 comma  // MOV sfr(0xF8), A
		}
	);

	uint8_t spl;
	tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x00);
	tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
	tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
	tick_amount(1); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
}

utester_define_test(rl78inst_move_a_addr16_test)
{
	init_env_flash_rom_and_display_info({
			"+------------------+-----------------+-------+----------+----------+" comma
			"| Instruction      | Template        | Bytes | Clocks 1 | Clocks 2 |" comma
			"+------------------+-----------------+-------+----------+----------+" comma
			"|  MOV A, !addr16  |  A <- (addr16)  |   3   |    1     |    4     |" comma
			"+------------------+-----------------+-------+----------+----------+" comma
		}, {
			0x8F comma 0xF0 comma 0x00 comma  // MOV A, addr16(0xF0, 0x00)
			0x8F comma 0xF0 comma 0x00 comma  // MOV A, addr16(0xF0, 0x00)
			0x8F comma 0xF0 comma 0x00 comma  // MOV A, addr16(0xF0, 0x00)
		}
	);

	// todo: handle both cases of clocks!
	utester_assert_true(false);

	{
		uint8_t a;
		tick_amount(4); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
		tick_amount(4); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
		rl78core_cpu_write_daddr(0xF0, 0x00, 0x0A); tick_amount(4); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
		rl78core_cpu_write_daddr(0xF0, 0x00, 0x00); tick_amount(4); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
		rl78core_cpu_write_daddr(0xF0, 0x00, 0x0A); tick_amount(4); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
		tick_amount(4); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
		tick_amount(4); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	}

	{
		uint8_t a;
		tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
		tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
		rl78core_cpu_write_daddr(0xF0, 0x00, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
		rl78core_cpu_write_daddr(0xF0, 0x00, 0x00); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
		rl78core_cpu_write_daddr(0xF0, 0x00, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
		tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
		tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	}
}

utester_define_test(rl78inst_move_addr16_a_test)
{
	init_env_flash_rom_and_display_info({
			"+------------------+-----------------+-------+----------+----------+" comma
			"| Instruction      | Template        | Bytes | Clocks 1 | Clocks 2 |" comma
			"+------------------+-----------------+-------+----------+----------+" comma
			"|  MOV !addr16, A  |  (addr16) <- A  |   3   |    1     |    -     |" comma
			"+------------------+-----------------+-------+----------+----------+" comma
		}, {
			0x9F comma 0xF0 comma 0x00 comma  // MOV addr16(0xF0, 0x00), A
			0x9F comma 0xF0 comma 0x00 comma  // MOV addr16(0xF0, 0x00), A
			0x9F comma 0xF0 comma 0x00 comma  // MOV addr16(0xF0, 0x00), A
		}
	);

	uint8_t value;
	tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x00);
	tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x0A);
	tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x0A);
	tick_amount(1); value = rl78core_cpu_read_daddr(0xF0, 0x00); utester_assert_equal(value, 0x0A);
}

utester_define_test(rl78inst_move_psw_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+------------------+---------------+-------+----------+----------+" comma
			"| Instruction      | Template      | Bytes | Clocks 1 | Clocks 2 |" comma
			"+------------------+---------------+-------+----------+----------+" comma
			"|  MOV PSW, #byte  |  PSW <- byte  |   3   |    3     |    -     |" comma
			"+------------------+---------------+-------+----------+----------+" comma
		}, {
			0xCE comma 0xFA comma 0x0A comma  // MOV PSW, 0x0A
			0xCE comma 0xFA comma 0x00 comma  // MOV PSW, 0x00
			0xCE comma 0xFA comma 0x0A comma  // MOV PSW, 0x0A
		}
	);

	uint8_t psw;
	tick_amount(3); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x00);
	tick_amount(3); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x00);
	tick_amount(3); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x0A);
	tick_amount(3); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x00);
	tick_amount(3); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x0A);
	tick_amount(3); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x0A);
	tick_amount(3); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x0A);
}

utester_define_test(rl78inst_move_a_psw_test)
{
	init_env_flash_rom_and_display_info({
			"+--------------+------------+-------+----------+----------+" comma
			"| Instruction  | Template   | Bytes | Clocks 1 | Clocks 2 |" comma
			"+--------------+------------+-------+----------+----------+" comma
			"|  MOV A, PSW  |  A <- PSW  |   2   |    1     |    -     |" comma
			"+--------------+------------+-------+----------+----------+" comma
		}, {
			0x8E comma 0xFA comma  // MOV A, PSW
			0x8E comma 0xFA comma  // MOV A, PSW
			0x8E comma 0xFA comma  // MOV A, PSW
		}
	);

	uint8_t a;
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_psw, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_sfr08(rl78core_sfr08_psw, 0x00); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_psw, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_psw_a_test)
{
	init_env_flash_rom_and_display_info({
			"+--------------+------------+-------+----------+----------+" comma
			"| Instruction  | Template   | Bytes | Clocks 1 | Clocks 2 |" comma
			"+--------------+------------+-------+----------+----------+" comma
			"|  MOV PSW, A  |  PSW <- A  |   2   |    3     |    -     |" comma
			"+--------------+------------+-------+----------+----------+" comma
		}, {
			0x9E comma 0xFA comma  // MOV PSW, A
			0x9E comma 0xFA comma  // MOV PSW, A
			0x9E comma 0xFA comma  // MOV PSW, A
		}
	);

	uint8_t pws;
	tick_amount(3); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x00);
	tick_amount(3); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(3); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); tick_amount(3); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(3); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x0A);
	tick_amount(3); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x0A);
	tick_amount(3); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x0A);
}

utester_define_test(rl78inst_move_es_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+-----------------+--------------+-------+----------+----------+" comma
			"| Instruction     | Template     | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-----------------+--------------+-------+----------+----------+" comma
			"|  MOV ES, #byte  |  ES <- byte  |   2   |    1     |    -     |" comma
			"+-----------------+--------------+-------+----------+----------+" comma
		}, {
			0x41 comma 0x0A comma  // MOV ES, 0x0A
			0x41 comma 0x00 comma  // MOV ES, 0x00
			0x41 comma 0x0A comma  // MOV ES, 0x0A
		}
	);

	uint8_t es;
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
}

utester_define_test(rl78inst_move_es_saddr_test)
{
	init_env_flash_rom_and_display_info({
			"+-----------------+-----------------+-------+----------+----------+" comma
			"| Instruction     | Template        | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-----------------+-----------------+-------+----------+----------+" comma
			"|  MOV ES, saddr  |  ES <- (saddr)  |   3   |    1     |    -     |" comma
			"+-----------------+-----------------+-------+----------+----------+" comma
		}, {
			0x61 comma 0xB8 comma 0x0A comma  // MOV ES, saddr(0x0A)
			0x61 comma 0xB8 comma 0x0A comma  // MOV ES, saddr(0x0A)
			0x61 comma 0xB8 comma 0x0A comma  // MOV ES, saddr(0x0A)
		}
	);

	uint8_t es;
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	rl78core_cpu_write_saddr(0x0A, 0x0A); tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	rl78core_cpu_write_saddr(0x0A, 0x00); tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	rl78core_cpu_write_saddr(0x0A, 0x0A); tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
}

utester_define_test(rl78inst_move_a_es_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+-----------+-------+----------+----------+" comma
			"| Instruction | Template  | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+-----------+-------+----------+----------+" comma
			"|  MOV A, ES  |  A <- ES  |   2   |    1     |    -     |" comma
			"+-------------+-----------+-------+----------+----------+" comma
		}, {
			0x8E comma 0xFD comma  // MOV A, ES
			0x8E comma 0xFD comma  // MOV A, ES
			0x8E comma 0xFD comma  // MOV A, ES
		}
	);

	uint8_t a;
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_es, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_sfr08(rl78core_sfr08_es, 0x00); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_es, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_es_a_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+-----------+-------+----------+----------+" comma
			"| Instruction | Template  | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+-----------+-------+----------+----------+" comma
			"|  MOV ES, A  |  ES <- A  |   2   |    1     |    -     |" comma
			"+-------------+-----------+-------+----------+----------+" comma
		}, {
			0x9E comma 0xFD comma  // MOV ES, A
			0x9E comma 0xFD comma  // MOV ES, A
			0x9E comma 0xFD comma  // MOV ES, A
		}
	);

	uint8_t es;
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	tick_amount(1); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
}

utester_define_test(rl78inst_move_cs_byte_test)
{
	init_env_flash_rom_and_display_info({
			"+-----------------+--------------+-------+----------+----------+" comma
			"| Instruction     | Template     | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-----------------+--------------+-------+----------+----------+" comma
			"|  MOV CS, #byte  |  CS <- byte  |   3   |    1     |    -     |" comma
			"+-----------------+--------------+-------+----------+----------+" comma
		}, {
			0xCE comma 0xFC comma 0x0A comma  // MOV CS, 0x0A
			0xCE comma 0xFC comma 0x00 comma  // MOV CS, 0x00
			0xCE comma 0xFC comma 0x0A comma  // MOV CS, 0x0A
		}
	);


	uint8_t cs;
	tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x00);
	tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x00);
	tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
	tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x00);
	tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
	tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
	tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
}

// ------------------------------------- Page 1 End ------------------------------------- //
// ------------------------------------ Page 2 Begin ------------------------------------ //

utester_define_test(rl78inst_move_a_cs_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+-----------+-------+----------+----------+" comma
			"| Instruction | Template  | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+-----------+-------+----------+----------+" comma
			"|  MOV A, CS  |  A <- CS  |   2   |    1     |    -     |" comma
			"+-------------+-----------+-------+----------+----------+" comma
		}, {
			0x8E comma 0xFC comma  // MOV A, CS
			0x8E comma 0xFC comma  // MOV A, CS
			0x8E comma 0xFC comma  // MOV A, CS
		}
	);

	uint8_t a;
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_cs, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_sfr08(rl78core_sfr08_cs, 0x00); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_cs, 0x0A); tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	tick_amount(1); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_cs_a_test)
{
	init_env_flash_rom_and_display_info({
			"+-------------+-----------+-------+----------+----------+" comma
			"| Instruction | Template  | Bytes | Clocks 1 | Clocks 2 |" comma
			"+-------------+-----------+-------+----------+----------+" comma
			"|  MOV CS, A  |  CS <- A  |   2   |    1     |    -     |" comma
			"+-------------+-----------+-------+----------+----------+" comma
		}, {
			0x9E comma 0xFC comma  // MOV CS, A
			0x9E comma 0xFC comma  // MOV CS, A
			0x9E comma 0xFC comma  // MOV CS, A
		}
	);

	uint8_t cs;
	tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x00);
	tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
	tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
	tick_amount(1); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
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
		&rl78inst_move_saddr_byte_test,
		&rl78inst_move_sfr_byte_test,
		&rl78inst_move_addr16_byte_test,
		&rl78inst_move_a_x_test,
		&rl78inst_move_a_c_test,
		&rl78inst_move_a_b_test,
		&rl78inst_move_a_e_test,
		&rl78inst_move_a_d_test,
		&rl78inst_move_a_l_test,
		&rl78inst_move_a_h_test,
		&rl78inst_move_x_a_test,
		&rl78inst_move_c_a_test,
		&rl78inst_move_b_a_test,
		&rl78inst_move_e_a_test,
		&rl78inst_move_d_a_test,
		&rl78inst_move_l_a_test,
		&rl78inst_move_h_a_test,
		&rl78inst_move_a_saddr_test,
		&rl78inst_move_saddr_a_test,
		&rl78inst_move_a_sfr_test,
		&rl78inst_move_sfr_a_test,
		&rl78inst_move_a_addr16_test,
		&rl78inst_move_addr16_a_test,
		&rl78inst_move_psw_byte_test,
		&rl78inst_move_a_psw_test,
		&rl78inst_move_psw_a_test,
		&rl78inst_move_es_byte_test,
		&rl78inst_move_es_saddr_test,
		&rl78inst_move_a_es_test,
		&rl78inst_move_es_a_test,
		&rl78inst_move_cs_byte_test,

		&rl78inst_move_a_cs_test,
		&rl78inst_move_cs_a_test,
);
