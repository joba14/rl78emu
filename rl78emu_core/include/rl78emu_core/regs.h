
/**
 * @file regs.h
 * 
 * @copyright This file is a part of the "rl78emu" project and is licensed, and
 * distributed under "rl78emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-03
 */

#ifndef __rl78emu_core__include__rl78emu_core__regs_h__
#define __rl78emu_core__include__rl78emu_core__regs_h__

#include "rl78emu_misc/common.h"

typedef union
{
	uint8_t value;

	struct
	{
		uint8_t n0 : 1;
		uint8_t n1 : 1;
		uint8_t n2 : 1;
		uint8_t n3 : 1;
		uint8_t n4 : 1;
		uint8_t n5 : 1;
		uint8_t n6 : 1;
		uint8_t n7 : 1;
	} bits;
} rl78f14_reg08_u;

typedef union
{
	uint16_t value;

	struct
	{
		uint8_t n0 : 1;
		uint8_t n1 : 1;
		uint8_t n2 : 1;
		uint8_t n3 : 1;
		uint8_t n4 : 1;
		uint8_t n5 : 1;
		uint8_t n6 : 1;
		uint8_t n7 : 1;
		uint8_t n8 : 1;
		uint8_t n9 : 1;
		uint8_t n10 : 1;
		uint8_t n11 : 1;
		uint8_t n12 : 1;
		uint8_t n13 : 1;
		uint8_t n14 : 1;
		uint8_t n15 : 1;
	} bits;
} rl78f14_reg16_u;

typedef union
{
	uint32_t value : 20;

	struct
	{
		uint8_t n0 : 1;
		uint8_t n1 : 1;
		uint8_t n2 : 1;
		uint8_t n3 : 1;
		uint8_t n4 : 1;
		uint8_t n5 : 1;
		uint8_t n6 : 1;
		uint8_t n7 : 1;
		uint8_t n8 : 1;
		uint8_t n9 : 1;
		uint8_t n10 : 1;
		uint8_t n11 : 1;
		uint8_t n12 : 1;
		uint8_t n13 : 1;
		uint8_t n14 : 1;
		uint8_t n15 : 1;
		uint8_t n16 : 1;
		uint8_t n17 : 1;
		uint8_t n18 : 1;
		uint8_t n19 : 1;
	} bits;
} rl78f14_reg20_u;

#endif
