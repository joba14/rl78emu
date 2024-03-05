
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

/**
 * @brief Initialize memory.
 */
void rl78core_mem_init(void);

/**
 * @brief Read 8-bit value from a provided address in the memory.
 * 
 * @param address address to read at
 * 
 * @return uint8_t read 8-bit value
 */
uint8_t rl78core_mem_read_u08(const uint20_t address);

/**
 * @brief Write 8-bit value into a provided address in the memory.
 * 
 * @param address address to write value at
 * @param value   value to write
 */
void rl78core_mem_write_u08(const uint20_t address, const uint8_t value);

/**
 * @brief Read 16-bit value from a provided address in the memory.
 * 
 * @note 16-bit value cannot be read at the end of a memory range. For example,
 * reading a 16-bit value at address 0x09 in a memory of size 10, would raise a
 * runtime error, since 16-bit value cannot be read from a memory range of less
 * than 2 bytes length.
 * 
 * @param address address to read at
 * 
 * @return uint16_t read 16-bit value
 */
uint16_t rl78core_mem_read_u16(const uint20_t address);

/**
 * @brief Write 16-bit value from a provided address in the memory.
 * 
 * @note 16-bit value cannot be written at the end of a memory range. For example,
 * reading a 16-bit value at address 0x09 in a memory of size 10, would raise a
 * runtime error, since 16-bit value cannot be read from a memory range of less
 * than 2 bytes length.
 * 
 * @param address address to write value at
 * @param value   value to write
 */
void rl78core_mem_write_u16(const uint20_t address, const uint16_t value);

#endif
