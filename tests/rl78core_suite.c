
/**
 * @file rl78core_suite.c
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

utester_define_test(rl78core_mem_read_u08_test)
{
	rl78core_mem_init();
	const uint8_t expected_value = 0;
	const uint20_t length = 5;

	for (uint20_t index = 0; index < length; ++index)
	{
		const uint8_t actual_value = rl78core_mem_read_u08(index);
		utester_assert_equal(actual_value, expected_value);
	}
}

utester_define_test(rl78core_mem_write_u08_test)
{
	rl78core_mem_init();
	const uint8_t expected_value = 0x0A;
	const uint20_t length = 5;

	for (uint20_t index = 0; index < length; ++index)
	{
		rl78core_mem_write_u08(index, expected_value);
	}

	for (uint20_t index = 0; index < length; ++index)
	{
		const uint8_t actual_value = rl78core_mem_read_u08(index);
		utester_assert_equal(actual_value, expected_value);
	}
}

utester_define_test(rl78core_mem_read_u16_test)
{
	rl78core_mem_init();
	const uint8_t expected_value = 0;
	const uint20_t length = 10;

	for (uint20_t index = 0; index < length; index += 2)
	{
		const uint16_t actual_value = rl78core_mem_read_u16(index);
		utester_assert_equal(actual_value, expected_value);
	}
}

utester_define_test(rl78core_mem_write_u16_test)
{
	rl78core_mem_init();
	const uint8_t expected_value = 0x0A;
	const uint20_t length = 10;

	for (uint20_t index = 0; index < length; index += 2)
	{
		rl78core_mem_write_u16(index, expected_value);
	}

	for (uint20_t index = 0; index < length; index += 2)
	{
		const uint16_t actual_value = rl78core_mem_read_u16(index);
		utester_assert_equal(actual_value, expected_value);
	}
}

utester_define_test(rl78core_cpu_read_pc_test)
{
	rl78core_cpu_init();
	const uint20_t pc_value = rl78core_cpu_read_pc();
	utester_assert_equal(pc_value, 0);
}

utester_define_test(rl78core_cpu_write_pc_test)
{
	rl78core_cpu_init();
	uint20_t pc_value = rl78core_cpu_read_pc();
	utester_assert_equal(pc_value, 0);
	rl78core_cpu_write_pc(10);
	pc_value = rl78core_cpu_read_pc();
	utester_assert_equal(pc_value, 10);
}

utester_define_test(rl78core_cpu_read_gpr08_test)
{
	rl78core_mem_init();
	rl78core_cpu_init();

	const uint8_t x_value = rl78core_cpu_read_gpr08(rl78core_gpr08_x);
	utester_assert_equal(x_value, 0);
	const uint8_t a_value = rl78core_cpu_read_gpr08(rl78core_gpr08_a);
	utester_assert_equal(a_value, 0);
	const uint8_t c_value = rl78core_cpu_read_gpr08(rl78core_gpr08_c);
	utester_assert_equal(c_value, 0);
	const uint8_t b_value = rl78core_cpu_read_gpr08(rl78core_gpr08_b);
	utester_assert_equal(b_value, 0);
	const uint8_t e_value = rl78core_cpu_read_gpr08(rl78core_gpr08_e);
	utester_assert_equal(e_value, 0);
	const uint8_t d_value = rl78core_cpu_read_gpr08(rl78core_gpr08_d);
	utester_assert_equal(d_value, 0);
	const uint8_t l_value = rl78core_cpu_read_gpr08(rl78core_gpr08_l);
	utester_assert_equal(l_value, 0);
	const uint8_t h_value = rl78core_cpu_read_gpr08(rl78core_gpr08_h);
	utester_assert_equal(h_value, 0);
}

utester_define_test(rl78core_cpu_write_gpr08_test)
{
	rl78core_mem_init();
	rl78core_cpu_init();

	uint8_t x_value = rl78core_cpu_read_gpr08(rl78core_gpr08_x);
	utester_assert_equal(x_value, 0);
	uint8_t a_value = rl78core_cpu_read_gpr08(rl78core_gpr08_a);
	utester_assert_equal(a_value, 0);
	uint8_t c_value = rl78core_cpu_read_gpr08(rl78core_gpr08_c);
	utester_assert_equal(c_value, 0);
	uint8_t b_value = rl78core_cpu_read_gpr08(rl78core_gpr08_b);
	utester_assert_equal(b_value, 0);
	uint8_t e_value = rl78core_cpu_read_gpr08(rl78core_gpr08_e);
	utester_assert_equal(e_value, 0);
	uint8_t d_value = rl78core_cpu_read_gpr08(rl78core_gpr08_d);
	utester_assert_equal(d_value, 0);
	uint8_t l_value = rl78core_cpu_read_gpr08(rl78core_gpr08_l);
	utester_assert_equal(l_value, 0);
	uint8_t h_value = rl78core_cpu_read_gpr08(rl78core_gpr08_h);
	utester_assert_equal(h_value, 0);

	rl78core_cpu_write_gpr08(rl78core_gpr08_x, 10);
	rl78core_cpu_write_gpr08(rl78core_gpr08_a, 10);
	rl78core_cpu_write_gpr08(rl78core_gpr08_c, 10);
	rl78core_cpu_write_gpr08(rl78core_gpr08_b, 10);
	rl78core_cpu_write_gpr08(rl78core_gpr08_e, 10);
	rl78core_cpu_write_gpr08(rl78core_gpr08_d, 10);
	rl78core_cpu_write_gpr08(rl78core_gpr08_l, 10);
	rl78core_cpu_write_gpr08(rl78core_gpr08_h, 10);
	
	x_value = rl78core_cpu_read_gpr08(rl78core_gpr08_x);
	utester_assert_equal(x_value, 10);
	a_value = rl78core_cpu_read_gpr08(rl78core_gpr08_a);
	utester_assert_equal(a_value, 10);
	c_value = rl78core_cpu_read_gpr08(rl78core_gpr08_c);
	utester_assert_equal(c_value, 10);
	b_value = rl78core_cpu_read_gpr08(rl78core_gpr08_b);
	utester_assert_equal(b_value, 10);
	e_value = rl78core_cpu_read_gpr08(rl78core_gpr08_e);
	utester_assert_equal(e_value, 10);
	d_value = rl78core_cpu_read_gpr08(rl78core_gpr08_d);
	utester_assert_equal(d_value, 10);
	l_value = rl78core_cpu_read_gpr08(rl78core_gpr08_l);
	utester_assert_equal(l_value, 10);
	h_value = rl78core_cpu_read_gpr08(rl78core_gpr08_h);
	utester_assert_equal(h_value, 10);
}

utester_define_test(rl78core_cpu_read_gpr16_test)
{
	rl78core_mem_init();
	rl78core_cpu_init();

	const uint16_t ax_value = rl78core_cpu_read_gpr16(rl78core_gpr16_ax);
	utester_assert_equal(ax_value, 0);
	const uint16_t bc_value = rl78core_cpu_read_gpr16(rl78core_gpr16_bc);
	utester_assert_equal(bc_value, 0);
	const uint16_t de_value = rl78core_cpu_read_gpr16(rl78core_gpr16_de);
	utester_assert_equal(de_value, 0);
	const uint16_t hl_value = rl78core_cpu_read_gpr16(rl78core_gpr16_hl);
	utester_assert_equal(hl_value, 0);
}

utester_define_test(rl78core_cpu_write_gpr16_test)
{
	rl78core_mem_init();
	rl78core_cpu_init();

	uint16_t ax_value = rl78core_cpu_read_gpr16(rl78core_gpr16_ax);
	utester_assert_equal(ax_value, 0);
	uint16_t bc_value = rl78core_cpu_read_gpr16(rl78core_gpr16_bc);
	utester_assert_equal(bc_value, 0);
	uint16_t de_value = rl78core_cpu_read_gpr16(rl78core_gpr16_de);
	utester_assert_equal(de_value, 0);
	uint16_t hl_value = rl78core_cpu_read_gpr16(rl78core_gpr16_hl);
	utester_assert_equal(hl_value, 0);

	rl78core_cpu_write_gpr16(rl78core_gpr16_ax, 0x0A0A);
	rl78core_cpu_write_gpr16(rl78core_gpr16_bc, 0x0A0A);
	rl78core_cpu_write_gpr16(rl78core_gpr16_de, 0x0A0A);
	rl78core_cpu_write_gpr16(rl78core_gpr16_hl, 0x0A0A);

	ax_value = rl78core_cpu_read_gpr16(rl78core_gpr16_ax);
	utester_assert_equal(ax_value, 0x0A0A);
	bc_value = rl78core_cpu_read_gpr16(rl78core_gpr16_bc);
	utester_assert_equal(bc_value, 0x0A0A);
	de_value = rl78core_cpu_read_gpr16(rl78core_gpr16_de);
	utester_assert_equal(de_value, 0x0A0A);
	hl_value = rl78core_cpu_read_gpr16(rl78core_gpr16_hl);
	utester_assert_equal(hl_value, 0x0A0A);
}

utester_run_suite(
	rl78core_suite,
		&rl78core_mem_read_u08_test,
		&rl78core_mem_write_u08_test,
		&rl78core_mem_read_u16_test,
		&rl78core_mem_write_u16_test,
		&rl78core_cpu_read_pc_test,
		&rl78core_cpu_write_pc_test,
		&rl78core_cpu_read_gpr08_test,
		&rl78core_cpu_write_gpr08_test,
		&rl78core_cpu_read_gpr16_test,
		&rl78core_cpu_write_gpr16_test,
);
