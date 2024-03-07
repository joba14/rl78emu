
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

/**
 * @brief Initialize cpu.
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
 * @brief Process a single tick with the cpu.
 */
void rl78core_cpu_tick(void);

#endif
