
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

#include "rl78core/reg.h"
#include "rl78core/mem.h"
#include "rl78core/cpu.h"

#include "./utester.h"

utester_define_test(rl78core_mem_read_u08_test)
{
	rl78core_mem_init();
	const uint20_t LENGTH = 5;

	for (uint20_t index = 0; index < LENGTH; ++index)
	{
		const uint8_t byte = rl78core_mem_read_u08(index);
		utester_assert_equal(byte, 0);
	}
}

utester_define_test(rl78core_mem_write_u08_test)
{
	rl78core_mem_init();
	const uint8_t BYTE = 0x0A;
	const uint20_t LENGTH = 5;

	for (uint20_t index = 0; index < LENGTH; ++index)
	{
		rl78core_mem_write_u08(index, BYTE);
	}

	for (uint20_t index = 0; index < LENGTH; ++index)
	{
		const uint8_t byte = rl78core_mem_read_u08(index);
		utester_assert_equal(byte, BYTE);
	}
}

utester_define_test(rl78core_mem_read_u16_test)
{
	rl78core_mem_init();
	const uint20_t LENGTH = 10;

	for (uint20_t index = 0; index < LENGTH; index += 2)
	{
		const uint16_t byte = rl78core_mem_read_u16(index);
		utester_assert_equal(byte, 0);
	}
}

utester_define_test(rl78core_mem_write_u16_test)
{
	rl78core_mem_init();
	const uint16_t BYTE = 0xA0;
	const uint20_t LENGTH = 10;

	for (uint20_t index = 0; index < LENGTH; index += 2)
	{
		rl78core_mem_write_u16(index, BYTE);
	}

	for (uint20_t index = 0; index < LENGTH; index += 2)
	{
		const uint16_t byte = rl78core_mem_read_u16(index);
		utester_assert_equal(byte, BYTE);
	}
}

utester_run_suite(
	rl78core_suite,
		&rl78core_mem_read_u08_test,
		&rl78core_mem_write_u08_test,
		&rl78core_mem_read_u16_test,
		&rl78core_mem_write_u16_test,
);
