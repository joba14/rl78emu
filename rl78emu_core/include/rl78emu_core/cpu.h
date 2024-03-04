
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

#ifndef __rl78emu_core__include__rl78emu_core__cpu_h__
#define __rl78emu_core__include__rl78emu_core__cpu_h__

#include "rl78emu_misc/common.h"

typedef enum
{
	rl78core_config_s1,
	rl78core_config_s2,
	rl78core_config_s3,
	rl78core_configs_count
} rl78core_config_e;

void rl78core_cpu_init(
	const rl78core_config_e config);

#endif
