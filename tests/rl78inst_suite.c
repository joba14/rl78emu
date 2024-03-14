
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

#define init_and_flash_rom(...)                                                \
	rl78core_mem_init();                                                       \
	rl78core_cpu_init();                                                       \
	                                                                           \
	const uint8_t rom[] = { __VA_ARGS__ };                                     \
	for (uint20_t address = 0; address < sizeof(rom); ++address)               \
	{                                                                          \
		rl78core_mem_write_u08(address, rom[address]);                         \
	}                                                                          \
	((void)0)

utester_define_test(rl78inst_move_x_byte_test)
{
	init_and_flash_rom(
		0x50, 0x0A,  // MOV X, 0x0A
		0x50, 0x00,  // MOV X, 0x00
		0x50, 0x0A,  // MOV X, 0x0A
	);

	uint8_t x;
	rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x00);
	rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x00);
	rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x0A);
	rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x00);
	rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x0A);
	rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x0A);
	rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x0A);
}

utester_define_test(rl78inst_move_a_byte_test)
{
	init_and_flash_rom(
		0x51, 0x0A,  // MOV A, 0x0A
		0x51, 0x00,  // MOV A, 0x00
		0x51, 0x0A,  // MOV A, 0x0A
	);

	uint8_t a;
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_c_byte_test)
{
	init_and_flash_rom(
		0x52, 0x0A,  // MOV C, 0x0A
		0x52, 0x00,  // MOV C, 0x00
		0x52, 0x0A,  // MOV C, 0x0A
	);

	uint8_t c;
	rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x00);
	rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x00);
	rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x0A);
	rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x00);
	rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x0A);
	rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x0A);
	rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x0A);
}

utester_define_test(rl78inst_move_b_byte_test)
{
	init_and_flash_rom(
		0x53, 0x0A,  // MOV B, 0x0A
		0x53, 0x00,  // MOV B, 0x00
		0x53, 0x0A,  // MOV B, 0x0A
	);

	uint8_t b;
	rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x00);
	rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x00);
	rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x0A);
	rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x00);
	rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x0A);
	rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x0A);
	rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x0A);
}

utester_define_test(rl78inst_move_e_byte_test)
{
	init_and_flash_rom(
		0x54, 0x0A,  // MOV E, 0x0A
		0x54, 0x00,  // MOV E, 0x00
		0x54, 0x0A,  // MOV E, 0x0A
	);

	uint8_t e;
	rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x00);
	rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x00);
	rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x0A);
	rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x00);
	rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x0A);
	rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x0A);
	rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x0A);
}

utester_define_test(rl78inst_move_d_byte_test)
{
	init_and_flash_rom(
		0x55, 0x0A,  // MOV D, 0x0A
		0x55, 0x00,  // MOV D, 0x00
		0x55, 0x0A,  // MOV D, 0x0A
	);

	uint8_t d;
	rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x00);
	rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x00);
	rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x0A);
	rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x00);
	rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x0A);
	rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x0A);
	rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x0A);
}

utester_define_test(rl78inst_move_l_byte_test)
{
	init_and_flash_rom(
		0x56, 0x0A,  // MOV L, 0x0A
		0x56, 0x00,  // MOV L, 0x00
		0x56, 0x0A,  // MOV L, 0x0A
	);

	uint8_t l;
	rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x00);
	rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x00);
	rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x0A);
	rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x00);
	rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x0A);
	rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x0A);
	rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x0A);
}

utester_define_test(rl78inst_move_h_byte_test)
{
	init_and_flash_rom(
		0x57, 0x0A,  // MOV H, 0x0A
		0x57, 0x00,  // MOV H, 0x00
		0x57, 0x0A,  // MOV H, 0x0A
	);

	uint8_t h;
	rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x00);
	rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x00);
	rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x0A);
	rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x00);
	rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x0A);
	rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x0A);
	rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x0A);
}

utester_define_test(rl78inst_move_saddr_byte_test)
{
	// todo: implement the test!
	allow_empty;
}

utester_define_test(rl78inst_move_sfr_byte_test)
{
	init_and_flash_rom(
		0xCE, 0xF8, 0x0A,  // MOV sfr(0xF8), 0x0A
		0xCE, 0xF8, 0x00,  // MOV sfr(0xF8), 0x00
		0xCE, 0xF8, 0x0A,  // MOV sfr(0xF8), 0x0A
	);

	uint8_t spl;
	rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x00);
	rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x00);
	rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
	rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x00);
	rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
	rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
	rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
}

utester_define_test(rl78inst_move_addr16_byte_test)
{
	init_and_flash_rom(
		0xCF, 0xF0, 0x00, 0x0A,  // MOV addr16(0xF0, 0x00), 0x0A
		0xCF, 0xF0, 0x00, 0x00,  // MOV addr16(0xF0, 0x00), 0x00
		0xCF, 0xF0, 0x00, 0x0A,  // MOV addr16(0xF0, 0x00), 0x0A
	);

	uint8_t value;
	rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x00);
	rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x00);
	rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x0A);
	rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x00);
	rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x0A);
	rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x0A);
	rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x0A);
}

utester_define_test(rl78inst_move_a_x_test)
{
	init_and_flash_rom(
		0x60,  // MOV A, X
		0x60,  // MOV A, X
		0x60,  // MOV A, X
	);

	uint8_t a;
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_x, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_x, 0x00); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_x, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_a_c_test)
{
	init_and_flash_rom(
		0x62,  // MOV A, C
		0x62,  // MOV A, C
		0x62,  // MOV A, C
	);

	uint8_t a;
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_c, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_c, 0x00); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_c, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_a_b_test)
{
	init_and_flash_rom(
		0x63,  // MOV A, B
		0x63,  // MOV A, B
		0x63,  // MOV A, B
	);

	uint8_t a;
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_b, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_b, 0x00); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_b, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_a_e_test)
{
	init_and_flash_rom(
		0x64,  // MOV A, E
		0x64,  // MOV A, E
		0x64,  // MOV A, E
	);

	uint8_t a;
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_e, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_e, 0x00); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_e, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_a_d_test)
{
	init_and_flash_rom(
		0x65,  // MOV A, D
		0x65,  // MOV A, D
		0x65,  // MOV A, D
	);

	uint8_t a;
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_d, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_d, 0x00); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_d, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_a_l_test)
{
	init_and_flash_rom(
		0x66,  // MOV A, L
		0x66,  // MOV A, L
		0x66,  // MOV A, L
	);

	uint8_t a;
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_l, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_l, 0x00); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_l, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_a_h_test)
{
	init_and_flash_rom(
		0x67,  // MOV A, H
		0x67,  // MOV A, H
		0x67,  // MOV A, H
	);

	uint8_t a;
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_h, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_h, 0x00); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_h, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_x_a_test)
{
	init_and_flash_rom(
		0x70,  // MOV X, A
		0x70,  // MOV X, A
		0x70,  // MOV X, A
	);

	uint8_t x;
	rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(x, 0x00);
	rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(x, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_x); utester_assert_equal(x, 0x0A);
	rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(x, 0x0A);
	rl78core_cpu_tick(); x = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(x, 0x0A);
}

utester_define_test(rl78inst_move_c_a_test)
{
	init_and_flash_rom(
		0x72,  // MOV C, A
		0x72,  // MOV C, A
		0x72,  // MOV C, A
	);

	uint8_t c;
	rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(c, 0x00);
	rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(c, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_c); utester_assert_equal(c, 0x0A);
	rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(c, 0x0A);
	rl78core_cpu_tick(); c = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(c, 0x0A);
}

utester_define_test(rl78inst_move_b_a_test)
{
	init_and_flash_rom(
		0x73,  // MOV B, A
		0x73,  // MOV B, A
		0x73,  // MOV B, A
	);

	uint8_t b;
	rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(b, 0x00);
	rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(b, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_b); utester_assert_equal(b, 0x0A);
	rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(b, 0x0A);
	rl78core_cpu_tick(); b = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(b, 0x0A);
}

utester_define_test(rl78inst_move_e_a_test)
{
	init_and_flash_rom(
		0x74,  // MOV E, A
		0x74,  // MOV E, A
		0x74,  // MOV E, A
	);

	uint8_t e;
	rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(e, 0x00);
	rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(e, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_e); utester_assert_equal(e, 0x0A);
	rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(e, 0x0A);
	rl78core_cpu_tick(); e = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(e, 0x0A);
}

utester_define_test(rl78inst_move_d_a_test)
{
	init_and_flash_rom(
		0x75,  // MOV D, A
		0x75,  // MOV D, A
		0x75,  // MOV D, A
	);

	uint8_t d;
	rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(d, 0x00);
	rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(d, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_d); utester_assert_equal(d, 0x0A);
	rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(d, 0x0A);
	rl78core_cpu_tick(); d = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(d, 0x0A);
}

utester_define_test(rl78inst_move_l_a_test)
{
	init_and_flash_rom(
		0x76,  // MOV L, A
		0x76,  // MOV L, A
		0x76,  // MOV L, A
	);

	uint8_t l;
	rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(l, 0x00);
	rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(l, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_l); utester_assert_equal(l, 0x0A);
	rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(l, 0x0A);
	rl78core_cpu_tick(); l = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(l, 0x0A);
}

utester_define_test(rl78inst_move_h_a_test)
{
	init_and_flash_rom(
		0x77,  // MOV H, A
		0x77,  // MOV H, A
		0x77,  // MOV H, A
	);

	uint8_t h;
	rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(h, 0x00);
	rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(h, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_h); utester_assert_equal(h, 0x0A);
	rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(h, 0x0A);
	rl78core_cpu_tick(); h = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(h, 0x0A);
}

utester_define_test(rl78inst_move_a_saddr_test)
{
	// todo: implement the test!
	allow_empty;
}

utester_define_test(rl78inst_move_saddr_a_test)
{
	// todo: implement the test!
	allow_empty;
}

utester_define_test(rl78inst_move_a_sfr_test)
{
	init_and_flash_rom(
		0x8E, 0xF8,  // MOV A, sfr(0xF8)
		0x8E, 0xF8,  // MOV A, sfr(0xF8)
		0x8E, 0xF8,  // MOV A, sfr(0xF8)
	);

	uint8_t a;
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_spl, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_sfr08(rl78core_sfr08_spl, 0x00); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_spl, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_sfr_a_test)
{
	init_and_flash_rom(
		0x9E, 0xF8,  // MOV sfr(0xF8), A
		0x9E, 0xF8,  // MOV sfr(0xF8), A
		0x9E, 0xF8,  // MOV sfr(0xF8), A
	);

	uint8_t spl;
	rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x00);
	rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
	rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
	rl78core_cpu_tick(); spl = rl78core_cpu_read_sfr08(rl78core_sfr08_spl); utester_assert_equal(spl, 0x0A);
}

utester_define_test(rl78inst_move_a_addr16_test)
{
	init_and_flash_rom(
		0x8F, 0xF0, 0x00,  // MOV A, addr16(0xF0, 0x00)
		0x8F, 0xF0, 0x00,  // MOV A, addr16(0xF0, 0x00)
		0x8F, 0xF0, 0x00,  // MOV A, addr16(0xF0, 0x00)
	);

	uint8_t a;
	rl78core_cpu_tick(); a = rl78core_mem_read_u08(0x00F0); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_mem_read_u08(0x00F0); utester_assert_equal(a, 0x00);
	rl78core_mem_write_u08(0x00F0, 0x0A); rl78core_cpu_tick(); a = rl78core_mem_read_u08(0x00F0); utester_assert_equal(a, 0x0A);
	rl78core_mem_write_u08(0x00F0, 0x00); rl78core_cpu_tick(); a = rl78core_mem_read_u08(0x00F0); utester_assert_equal(a, 0x00);
	rl78core_mem_write_u08(0x00F0, 0x0A); rl78core_cpu_tick(); a = rl78core_mem_read_u08(0x00F0); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_mem_read_u08(0x00F0); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_mem_read_u08(0x00F0); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_addr16_a_test)
{
	init_and_flash_rom(
		0x9F, 0xF0, 0x00,  // MOV addr16(0xF0, 0x00), A
		0x9F, 0xF0, 0x00,  // MOV addr16(0xF0, 0x00), A
		0x9F, 0xF0, 0x00,  // MOV addr16(0xF0, 0x00), A
	);

	uint8_t value;
	rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x00);
	rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x0A);
	rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x0A);
	rl78core_cpu_tick(); value = rl78core_mem_read_u08(0x00F0); utester_assert_equal(value, 0x0A);
}

utester_define_test(rl78inst_move_psw_byte_test)
{
	init_and_flash_rom(
		0xCE, 0xFA, 0x0A,  // MOV PSW, 0x0A
		0xCE, 0xFA, 0x00,  // MOV PSW, 0x00
		0xCE, 0xFA, 0x0A,  // MOV PSW, 0x0A
	);

	uint8_t psw;
	rl78core_cpu_tick(); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x00);
	rl78core_cpu_tick(); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x00);
	rl78core_cpu_tick(); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x0A);
	rl78core_cpu_tick(); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x00);
	rl78core_cpu_tick(); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x0A);
	rl78core_cpu_tick(); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x0A);
	rl78core_cpu_tick(); psw = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(psw, 0x0A);
}

utester_define_test(rl78inst_move_a_psw_test)
{
	init_and_flash_rom(
		0x8E, 0xFA,  // MOV A, PSW
		0x8E, 0xFA,  // MOV A, PSW
		0x8E, 0xFA,  // MOV A, PSW
	);

	uint8_t a;
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_psw, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_sfr08(rl78core_sfr08_psw, 0x00); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_psw, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_psw_a_test)
{
	init_and_flash_rom(
		0x9E, 0xFA,  // MOV PSW, A
		0x9E, 0xFA,  // MOV PSW, A
		0x9E, 0xFA,  // MOV PSW, A
	);

	uint8_t pws;
	rl78core_cpu_tick(); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x00);
	rl78core_cpu_tick(); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); rl78core_cpu_tick(); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x0A);
	rl78core_cpu_tick(); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x0A);
	rl78core_cpu_tick(); pws = rl78core_cpu_read_sfr08(rl78core_sfr08_psw); utester_assert_equal(pws, 0x0A);
}

utester_define_test(rl78inst_move_es_byte_test)
{
	init_and_flash_rom(
		0x41, 0x0A,  // MOV ES, 0x0A
		0x41, 0x00,  // MOV ES, 0x00
		0x41, 0x0A,  // MOV ES, 0x0A
	);

	uint8_t es;
	rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
}

utester_define_test(rl78inst_move_es_saddr_test)
{
	// todo: implement the test!
	allow_empty;
}

utester_define_test(rl78inst_move_a_es_test)
{
	init_and_flash_rom(
		0x8E, 0xFD,  // MOV A, ES
		0x8E, 0xFD,  // MOV A, ES
		0x8E, 0xFD,  // MOV A, ES
	);

	uint8_t a;
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_es, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_sfr08(rl78core_sfr08_es, 0x00); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_es, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_es_a_test)
{
	init_and_flash_rom(
		0x9E, 0xFD,  // MOV ES, A
		0x9E, 0xFD,  // MOV ES, A
		0x9E, 0xFD,  // MOV ES, A
	);

	uint8_t es;
	rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
	rl78core_cpu_tick(); es = rl78core_cpu_read_sfr08(rl78core_sfr08_es); utester_assert_equal(es, 0x0A);
}

utester_define_test(rl78inst_move_cs_byte_test)
{
	init_and_flash_rom(
		0xCE, 0xFC, 0x0A,  // MOV CS, 0x0A
		0xCE, 0xFC, 0x00,  // MOV CS, 0x00
		0xCE, 0xFC, 0x0A,  // MOV CS, 0x0A
	);

	uint8_t cs;
	rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x00);
	rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x00);
	rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
	rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x00);
	rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
	rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
	rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
}

// ------------------------------------- Page 1 End ------------------------------------- //
// ------------------------------------ Page 2 Begin ------------------------------------ //

utester_define_test(rl78inst_move_a_cs_test)
{
	init_and_flash_rom(
		0x8E, 0xFC,  // MOV A, CS
		0x8E, 0xFC,  // MOV A, CS
		0x8E, 0xFC,  // MOV A, CS
	);

	uint8_t a;
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_cs, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_write_sfr08(rl78core_sfr08_cs, 0x00); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x00);
	rl78core_cpu_write_sfr08(rl78core_sfr08_cs, 0x0A); rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
	rl78core_cpu_tick(); a = rl78core_cpu_read_gpr08(rl78core_gpr08_a); utester_assert_equal(a, 0x0A);
}

utester_define_test(rl78inst_move_cs_a_test)
{
	init_and_flash_rom(
		0x9E, 0xFC,  // MOV CS, A
		0x9E, 0xFC,  // MOV CS, A
		0x9E, 0xFC,  // MOV CS, A
	);

	uint8_t cs;
	rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x00);
	rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x00); rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x00);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 0x0A); rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
	rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
	rl78core_cpu_tick(); cs = rl78core_cpu_read_sfr08(rl78core_sfr08_cs); utester_assert_equal(cs, 0x0A);
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
