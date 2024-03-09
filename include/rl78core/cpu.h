
/**
 * @file cpu.h
 * 
 * @copyright This file is a part of the "rl78emu" project and is licensed, and
 * distributed under "rl78emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-03
 */

#ifndef __rl78emu__include__rl78core__cpu_h__
#define __rl78emu__include__rl78core__cpu_h__

#include "rl78misc/common.h"

#define rl78core_gpr08_x 0x00
#define rl78core_gpr08_a 0x01
#define rl78core_gpr08_c 0x02
#define rl78core_gpr08_b 0x03
#define rl78core_gpr08_e 0x04
#define rl78core_gpr08_d 0x05
#define rl78core_gpr08_l 0x06
#define rl78core_gpr08_h 0x07
#define rl78core_gpr08s_count 0x08

#define rl78core_gpr16_ax 0x00
#define rl78core_gpr16_bc 0x02
#define rl78core_gpr16_de 0x04
#define rl78core_gpr16_hl 0x06
#define rl78core_gpr16s_count 0x08

// todo: define all the sfrs here as offsets in their respective addressing ranges and functions to read and write.

/**
 * @brief Initialize the cpu.
 */
void rl78core_cpu_init(void);

/**
 * @brief Read the 10-bit value of the pc register.
 * 
 * @param value value to write to the pc register
 */
uint20_t rl78core_cpu_read_pc(void);

/**
 * @brief Read the 10-bit value of the pc register.
 */
void rl78core_cpu_write_pc(const uint20_t value);

/**
 * @brief Read the 8-bit value of provided general purpose register.
 * 
 * @param gpr08 offset of a 8-bit general purpose register in a bank
 * 
 * @return uint8_t value of the provided register
 */
uint8_t rl78core_cpu_read_gpr08(const uint8_t gpr08);

/**
 * @brief Write the 8-bit value to provided general purpose register.
 * 
 * @param gpr08 offset of a 8-bit general purpose register in a bank
 * @param value value to write to the provided register
 */
void rl78core_cpu_write_gpr08(const uint8_t gpr08, const uint8_t value);

/**
 * @brief Read the 16-bit value of provided general purpose register.
 * 
 * @param gpr16 offset of a 16-bit general purpose register in a bank
 * 
 * @return uint16_t value of the provided register
 */
uint16_t rl78core_cpu_read_gpr16(const uint8_t gpr16);

/**
 * @brief Write the 16-bit value of provided general purpose register.
 * 
 * @param gpr16 offset of a 16-bit general purpose register in a bank
 * @param value value to write to the provided register
 */
void rl78core_cpu_write_gpr16(const uint8_t gpr16, const uint16_t value);

/**
 * @brief Halt the cpu.
 */
void rl78core_cpu_halt(void);

/**
 * @brief Check if cpu is halted or not.
 * 
 * @return bool_t halted flag
 */
bool_t rl78core_cpu_halted(void);

/**
 * @brief Process a single tick with the cpu.
 */
void rl78core_cpu_tick(void);

#endif
