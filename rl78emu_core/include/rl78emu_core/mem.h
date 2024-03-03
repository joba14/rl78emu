
/**
 * @file mem.h
 * 
 * @copyright This file is a part of the "rl78emu" project and is licensed, and
 * distributed under "rl78emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-03
 */

#ifndef __rl78emu_core__include__rl78emu_core__mem_h__
#define __rl78emu_core__include__rl78emu_core__mem_h__

#include "rl78emu_misc/common.h"

void rl78f14_mem_init(
	void);

uint8_t rl78f14_mem_read_u08(
	const uint32_t address);

void rl78f14_mem_write_u08(
	const uint32_t address,
	const uint8_t value);

uint16_t rl78f14_mem_read_u16(
	const uint32_t address);

void rl78f14_mem_write_u16(
	const uint32_t address,
	const uint16_t value);

uint8_t rl78f14_mem_read_u08_from_cfm(
	const uint32_t address);

void rl78f14_mem_write_u08_to_cfm(
	const uint32_t address,
	const uint8_t value);

uint16_t rl78f14_mem_read_u16_from_cfm(
	const uint32_t address);

void rl78f14_mem_write_u16_to_cfm(
	const uint32_t address,
	const uint16_t value);

uint8_t rl78f14_mem_read_u08_from_dfm(
	const uint32_t address);

void rl78f14_mem_write_u08_to_dfm(
	const uint32_t address,
	const uint8_t value);

uint16_t rl78f14_mem_read_u16_from_dfm(
	const uint32_t address);

void rl78f14_mem_write_u16_to_dfm(
	const uint32_t address,
	const uint16_t value);

uint8_t rl78f14_mem_read_u08_from_ram(
	const uint32_t address);

void rl78f14_mem_write_u08_to_ram(
	const uint32_t address,
	const uint8_t value);

uint16_t rl78f14_mem_read_u16_from_ram(
	const uint32_t address);

void rl78f14_mem_write_u16_to_ram(
	const uint32_t address,
	const uint16_t value);

#endif
