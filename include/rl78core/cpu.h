
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

#define rl78core_gpr08_x    0x00
#define rl78core_gpr08_a    0x01
#define rl78core_gpr08_c    0x02
#define rl78core_gpr08_b    0x03
#define rl78core_gpr08_e    0x04
#define rl78core_gpr08_d    0x05
#define rl78core_gpr08_l    0x06
#define rl78core_gpr08_h    0x07
#define rl78core_gpr16_ax   0x00
#define rl78core_gpr16_bc   0x02
#define rl78core_gpr16_de   0x04
#define rl78core_gpr16_hl   0x06
#define rl78core_gprs_count 0x08

// note: the least significant bit must always be cleared to 0.
#define rl78core_sfr08_spl  0xF8  // 0xFFFF8 - 0xFFF00
#define rl78core_sfr08_sph  0xF9  // 0xFFFF9 - 0xFFF00
// +----+---+------+----+------+------+------+----+
// | IE | Z | RBS1 | AC | RBS0 | ISP1 | ISP0 | CY |
// +----+---+------+----+------+------+------+----+
#define rl78core_sfr08_psw  0xFA  // 0xFFFFA - 0xFFF00
#define rl78core_sfr08_cs   0xFC  // 0xFFFFC - 0xFFF00
#define rl78core_sfr08_es   0xFD  // 0xFFFFD - 0xFFF00
#define rl78core_sfr08_pmc  0xFE  // 0xFFFFE - 0xFFF00
#define rl78core_sfr08_mem  0xFF  // 0xFFFFF - 0xFFF00
#define rl78core_sfrs_count 0x100
// todo: define all the sfrs here as offsets in their respective addressing ranges and functions to read and write.

/**
 * @brief Initialize the cpu.
 */
void rl78core_cpu_init(void);

/**
 * @brief Reset the cpu.
 */
void rl78core_cpu_reset(void);

/**
 * @brief Halt the cpu.
 */
void rl78core_cpu_halt(void);

/**
 * @brief Check if cpu is halted or not.
 * 
 * @return bool_t halted flag
 */
bool_t rl78core_cpu_halted(void) nodiscard;

/**
 * @brief Read the 20-bit value of the pc register.
 * 
 * @param value value to write to the pc register
 */
uint20_t rl78core_cpu_pc_read(void) nodiscard;

/**
 * @brief Read the 20-bit value of the pc register.
 */
void rl78core_cpu_pc_write(const uint20_t value);

/**
 * @brief Get the absolute 20-bit address of a provided 8-bit general purpose
 * register.
 * 
 * @param gpr08 8-bit general purpose register
 * 
 * @return uint20_t absolute address
 */
uint20_t rl78core_cpu_gpr08_to_absolute_address(const uint8_t gpr08) nodiscard;

/**
 * @brief Get the absolute 20-bit address of a provided 16-bit general purpose
 * register.
 * 
 * @param gpr16 16-bit general purpose register
 * 
 * @return uint20_t absolute address
 */
uint20_t rl78core_cpu_gpr16_to_absolute_address(const uint8_t gpr16) nodiscard;

/**
 * @brief Get the absolute 20-bit address of a provided 8-bit special function
 * register.
 * 
 * @param sfr08 8-bit special function register
 * 
 * @return uint20_t absolute address
 */
uint20_t rl78core_cpu_sfr08_to_absolute_address(const uint8_t sfr08) nodiscard;

/**
 * @brief Get the absolute 20-bit address of a provided 16-bit special function
 * register.
 * 
 * @param sfr16 16-bit special function register
 * 
 * @return uint20_t absolute address
 */
uint20_t rl78core_cpu_sfr16_to_absolute_address(const uint8_t sfr16) nodiscard;

/**
 * @brief Process a single tick with the cpu.
 */
void rl78core_cpu_tick(void);

#endif
