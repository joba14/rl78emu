
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

#include "rl78misc/debug.h"
#include "rl78misc/logger.h"

#include "rl78core/mem.h"

#define rl78core_mem_flash_capacity 0x100000

typedef struct
{
	uint8_t* flash;
} rl78core_mem_s;

static rl78core_mem_s g_rl78core_mem;

/**
 * @brief Reference memory at a provided address. It requires the size in bytes
 * of the referenceable type for safety checks.
 * 
 * @param address address to reference memory at
 * @param size    referenceable type size in bytes
 * 
 * @return uint8_t* pointer to the referenced memory
 */
static uint8_t* reference_mem_at(const uint20_t address, const uint20_t size) nodiscard;

void rl78core_mem_init(void)
{
	g_rl78core_mem.flash = (uint8_t* const)rl78misc_malloc(rl78core_mem_flash_capacity * sizeof(uint8_t));
	rl78misc_debug_assert(g_rl78core_mem.flash != NULL);
	rl78core_mem_reset();
}

void rl78core_mem_reset(void)
{
	// todo: finish implementing!
}

uint8_t rl78core_mem_read_u08(const uint20_t address)
{
	const uint8_t* const base = reference_mem_at(address, sizeof(uint8_t));
	rl78misc_debug_assert(base != NULL);
	return (*base & 0xFF);
}

void rl78core_mem_write_u08(const uint20_t address, const uint8_t value)
{
	uint8_t* const base = reference_mem_at(address, sizeof(uint8_t));
	rl78misc_debug_assert(base != NULL);
	*base = (value & 0xFF);
}

uint16_t rl78core_mem_read_u16(const uint20_t address)
{
	const uint8_t* const base = reference_mem_at(address, sizeof(uint16_t));
	rl78misc_debug_assert(base != NULL);

	return (uint16_t)((uint16_t)(*(base + 0) & 0x00FF) | \
	(uint16_t)((uint16_t)(*(base + 1) << 8) & 0xFF00));
}

void rl78core_mem_write_u16(const uint20_t address, const uint16_t value)
{
	uint8_t* const base = reference_mem_at(address, sizeof(uint16_t));
	rl78misc_debug_assert(base != NULL);

	*(base + 0) = (uint8_t)(value & 0x00FF);
	*(base + 1) = (uint8_t)((uint16_t)(value >> 8) & 0x00FF);
}

static uint8_t* reference_mem_at(const uint20_t address, const uint20_t size)
{
	rl78misc_debug_assert(size > 0);
	rl78misc_debug_assert(address < (rl78core_mem_flash_capacity - size + 1));
	uint8_t* const base = &g_rl78core_mem.flash[address];
	rl78misc_debug_assert(base != NULL);
	return base;
}
