
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

typedef struct
{
	bool_t halted;
	uint20_t pc;

	uint8_t x, a, c, b, e, d, l, h;
	uint16_t ax, bc, de, hl;

	uint8_t spl, sph, psw, cs, es, pmc, mem;
} rl78core_cpu_stats_s;

/**
 * @brief Initialize the cpu.
 */
void rl78core_cpu_init(void);

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
 * @brief Read the 8-bit value of provided special function register.
 * 
 * @param sfr08 offset of a 8-bit special function register
 * 
 * @return uint8_t value of the provided register
 */
uint8_t rl78core_cpu_read_sfr08(const uint8_t sfr08);

/**
 * @brief Write the 8-bit value to provided special function register.
 * 
 * @param sfr08 offset of a 8-bit special function register
 * @param value value to write to the provided register
 */
void rl78core_cpu_write_sfr08(const uint8_t sfr08, const uint8_t value);

/**
 * @brief Read the 16-bit value of provided special function register.
 * 
 * @param sfr16 offset of a 16-bit special function register
 * 
 * @return uint16_t value of the provided register
 */
uint16_t rl78core_cpu_read_sfr16(const uint8_t sfr16);

/**
 * @brief Write the 16-bit value of provided special function register.
 * 
 * @param sfr16 offset of a 16-bit special function register
 * @param value value to write to the provided register
 */
void rl78core_cpu_write_sfr16(const uint8_t sfr16, const uint16_t value);

/**
 * @brief Get the statistics of the cpu.
 * 
 * @param stats cpu statistics
 */
void rl78core_cpu_get_stats(rl78core_cpu_stats_s* const stats);

/**
 * @brief Process a single tick with the cpu.
 */
void rl78core_cpu_tick(void);

#endif
