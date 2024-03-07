
/**
 * @file rl78core_suite.c
 * 
 * @copyright This file is a part of the "rl78emu" project and is licensed, and
 * distributed under "rl78emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-07
 */

#include "rl78misc/common.h"
#include "rl78misc/debug.h"
#include "rl78misc/logger.h"

#include "rl78core/reg.h"
#include "rl78core/mem.h"
#include "rl78core/cpu.h"

#include "./utester.h"

utester_define_test(test1)
{
	utester_assert_true(1 == 1);
}

utester_define_test(test2)
{
	utester_assert_true(1 != 2);
}

utester_define_test(test3)
{
	utester_assert_true(2 == 2);
}

utester_run_suite(rl78core_suite, &test1, &test2, &test3);
