
/**
 * @file debug.h
 * 
 * @copyright This file is a part of the "rl78emu" project and is licensed, and
 * distributed under "rl78emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-03
 */

#ifndef __rl78emu__include__rl78misc__debug_h__
#define __rl78emu__include__rl78misc__debug_h__

#ifndef NDEBUG
#	include "rl78misc/common.h"

/**
 * @brief Debug assert implementation.
 * 
 * @param expression        expression to evaluate
 * @param expression_string stringified expression
 * @param file              file in which assert exists
 * @param line              line in which assert exists
 */
void rl78misc_debug_assert_impl(const bool_t expression, const char_t* const expression_string, const char_t* const file, const uint64_t line);

/**
 * @brief Debug assert wrapper.
 * 
 * @note It abstracts the @ref rl78misc_debug_assert_impl function and passes line,
 * file, and stringified expression to the implementation function.
 */
#	define rl78misc_debug_assert(_expression)                                  \
		rl78misc_debug_assert_impl(                                            \
			_expression,                                                       \
			#_expression,                                                      \
			(const char_t* const)__FILE__,                                     \
			(const uint64_t)__LINE__                                           \
		)
#else
/**
 * @brief Debug assert wrapper.
 * 
 * @note It abstracts the @ref rl78misc_debug_assert_impl function and passes line,
 * file, and stringified expression to the implementation function.
 */
#	define rl78misc_debug_assert(_expression) ((void)(_expression))
#endif

#endif
