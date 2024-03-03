
/**
 * @file mem.c
 * 
 * @copyright This file is a part of the "rl78emu" project and is licensed, and
 * distributed under "rl78emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-03
 */

#include "rl78emu_misc/debug.h"
#include "rl78emu_misc/logger.h"

#include "rl78emu_core/regs.h"
#include "rl78emu_core/mem.h"

#define rl78f14_mem_flash_capacity 0x100000

#define rl78f14_mem_flash_cfm_start 0xF0000  // todo: figure out the actual values here!
#define rl78f14_mem_flash_cfm_length 0x1000  // todo: figure out the actual values here!

#define rl78f14_mem_flash_dfm_start 0xF0000  // todo: figure out the actual values here!
#define rl78f14_mem_flash_dfm_length 0x1000  // todo: figure out the actual values here!

#define rl78f14_mem_flash_ram_start 0xF0000  // todo: figure out the actual values here!
#define rl78f14_mem_flash_ram_length 0x1000  // todo: figure out the actual values here!

typedef struct
{
	uint8_t flash[rl78f14_mem_flash_capacity];
} rl78f14_mem_s;

static rl78f14_mem_s g_rl78f14_mem;

void rl78f14_mem_init(
	void)
{
}

uint8_t rl78f14_mem_read_u08(
	const uint32_t address)
{
	rl78emu_misc_debug_assert(address < rl78f14_mem_flash_capacity);
	return (g_rl78f14_mem.flash[address] & 0xFF);
}

void rl78f14_mem_write_u08(
	const uint32_t address,
	const uint8_t value)
{
	rl78emu_misc_debug_assert(address < rl78f14_mem_flash_capacity);
	g_rl78f14_mem.flash[address] = (value & 0xFF);
}

uint16_t rl78f14_mem_read_u16(
	const uint32_t address)
{
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_capacity - 1));
	return (uint16_t)(
		(uint16_t)(g_rl78f14_mem.flash[address + 0] & 0x00FF) | \
		(uint16_t)((uint16_t)(g_rl78f14_mem.flash[address + 1] << 8) & 0xFF00)
	);
}

void rl78f14_mem_write_u16(
	const uint32_t address,
	const uint16_t value)
{
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_capacity - 1));
	g_rl78f14_mem.flash[address + 0] = (uint8_t)(value & 0x00FF);
	g_rl78f14_mem.flash[address + 1] = (uint8_t)((uint16_t)(value >> 8) & 0x00FF);
}

inline uint8_t rl78f14_mem_read_u08_from_cfm(
	const uint32_t address)
{
	rl78emu_misc_debug_assert(address >= rl78f14_mem_flash_cfm_start);
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_cfm_start + rl78f14_mem_flash_cfm_length));
	return rl78f14_mem_read_u08(address);
}

inline void rl78f14_mem_write_u08_to_cfm(
	const uint32_t address,
	const uint8_t value)
{
	rl78emu_misc_debug_assert(address >= rl78f14_mem_flash_cfm_start);
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_cfm_start + rl78f14_mem_flash_cfm_length));
	rl78f14_mem_write_u08(address, value);
}

inline uint16_t rl78f14_mem_read_u16_from_cfm(
	const uint32_t address)
{
	rl78emu_misc_debug_assert(address >= rl78f14_mem_flash_cfm_start);
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_cfm_start + rl78f14_mem_flash_cfm_length - 1));
	return rl78f14_mem_read_u16(address);
}

inline void rl78f14_mem_write_u16_to_cfm(
	const uint32_t address,
	const uint16_t value)
{
	rl78emu_misc_debug_assert(address >= rl78f14_mem_flash_cfm_start);
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_cfm_start + rl78f14_mem_flash_cfm_length - 1));
	rl78f14_mem_write_u16(address, value);
}

inline uint8_t rl78f14_mem_read_u08_from_dfm(
	const uint32_t address)
{
	rl78emu_misc_debug_assert(address >= rl78f14_mem_flash_dfm_start);
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_dfm_start + rl78f14_mem_flash_dfm_length));
	return rl78f14_mem_read_u08(address);
}

inline void rl78f14_mem_write_u08_to_dfm(
	const uint32_t address,
	const uint8_t value)
{
	rl78emu_misc_debug_assert(address >= rl78f14_mem_flash_dfm_start);
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_dfm_start + rl78f14_mem_flash_dfm_length));
	rl78f14_mem_write_u08(address, value);
}

inline uint16_t rl78f14_mem_read_u16_from_dfm(
	const uint32_t address)
{
	rl78emu_misc_debug_assert(address >= rl78f14_mem_flash_dfm_start);
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_dfm_start + rl78f14_mem_flash_dfm_length - 1));
	return rl78f14_mem_read_u16(address);
}

inline void rl78f14_mem_write_u16_to_dfm(
	const uint32_t address,
	const uint16_t value)
{
	rl78emu_misc_debug_assert(address >= rl78f14_mem_flash_dfm_start);
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_dfm_start + rl78f14_mem_flash_dfm_length - 1));
	rl78f14_mem_write_u16(address, value);
}

inline uint8_t rl78f14_mem_read_u08_from_ram(
	const uint32_t address)
{
	rl78emu_misc_debug_assert(address >= rl78f14_mem_flash_ram_start);
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_ram_start + rl78f14_mem_flash_ram_length));
	return rl78f14_mem_read_u08(address);
}

inline void rl78f14_mem_write_u08_to_ram(
	const uint32_t address,
	const uint8_t value)
{
	rl78emu_misc_debug_assert(address >= rl78f14_mem_flash_ram_start);
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_ram_start + rl78f14_mem_flash_ram_length));
	rl78f14_mem_write_u08(address, value);
}

inline uint16_t rl78f14_mem_read_u16_from_ram(
	const uint32_t address)
{
	rl78emu_misc_debug_assert(address >= rl78f14_mem_flash_ram_start);
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_ram_start + rl78f14_mem_flash_ram_length - 1));
	return rl78f14_mem_read_u16(address);
}

inline void rl78f14_mem_write_u16_to_ram(
	const uint32_t address,
	const uint16_t value)
{
	rl78emu_misc_debug_assert(address >= rl78f14_mem_flash_ram_start);
	rl78emu_misc_debug_assert(address < (rl78f14_mem_flash_ram_start + rl78f14_mem_flash_ram_length - 1));
	rl78f14_mem_write_u16(address, value);
}
