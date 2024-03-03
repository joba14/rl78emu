
/**
 * @file config.h
 * 
 * @copyright This file's a part of the "rl78f14emu" project and is distributed
 * under "rl78f14emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-02
 */

#ifndef __rl78emu_cli__include__rl78emu_cli__config_h__
#define __rl78emu_cli__include__rl78emu_cli__config_h__

#include "rl78emu_misc/common.h"

typedef struct
{
	const char_t* binary;
} rl78emu_cli_config_s;

/**
 * @brief Create config object with provided command line arguments.
 * 
 * @param argc command line arguments count
 * @param argv command line arguments pointer
 * 
 * @return rl78emu_cli_config_s
 */
rl78emu_cli_config_s rl78emu_cli_config_from_cli(
	const uint64_t argc,
	const char_t** const argv);

/**
 * @brief Print the usage banner.
 */
void rl78emu_cli_config_usage(
	void);

#endif
