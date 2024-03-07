
/**
 * @file logger.h
 * 
 * @copyright This file is a part of the "rl78emu" project and is licensed, and
 * distributed under "rl78emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-03
 */

#ifndef __rl78emu__include__rl78misc__logger_h__
#define __rl78emu__include__rl78misc__logger_h__

#include "rl78misc/common.h"

/**
 * @brief Log tagless level formattable messages.
 * 
 * @param format format of the log
 * @param ...    arguments of the log
 */
void rl78misc_logger_log(
	const char_t* const format,
	...) __attribute__ ((format (printf, 1, 2)));

/**
 * @brief Log info level formattable messages.
 * 
 * @param format format of the log
 * @param ...    arguments of the log
 */
void rl78misc_logger_info(
	const char_t* const format,
	...) __attribute__ ((format (printf, 1, 2)));

/**
 * @brief Log warn level formattable messages.
 * 
 * @param format format of the log
 * @param ...    arguments of the log
 */
void rl78misc_logger_warn(
	const char_t* const format,
	...) __attribute__ ((format (printf, 1, 2)));

/**
 * @brief Log error level formattable messages.
 * 
 * @param format format of the log
 * @param ...    arguments of the log
 */
void rl78misc_logger_error(
	const char_t* const format,
	...) __attribute__ ((format (printf, 1, 2)));

#endif
