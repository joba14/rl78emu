
/**
 * @file rl78misc_suite.c
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

#include "./utester.h"

/**
 * @brief Construct a new utester define test for rl78misc_malloc.
 */
utester_define_test(rl78misc_malloc_test)
{
	const void* const pointer = rl78misc_malloc(10);
	utester_assert_not_equal(pointer, NULL);
	rl78misc_free(pointer);
}

/**
 * @brief Construct a new utester define test for rl78misc_realloc.
 */
utester_define_test(rl78misc_realloc_test)
{
	const void* const pointer = rl78misc_realloc(NULL, 10);
	utester_assert_not_equal(pointer, NULL);
	rl78misc_free(pointer);
}

/**
 * @brief Construct a new utester define test for rl78misc_free.
 */
utester_define_test(rl78misc_free_test)
{
	const void* pointer = rl78misc_malloc(10);
	utester_assert_not_equal(pointer, NULL);
	pointer = rl78misc_free(pointer);
	utester_assert_equal(pointer, NULL);
}

/**
 * @brief Construct a new utester define test for rl78misc_memset.
 */
utester_define_test(rl78misc_memset_test)
{
	uint8_t buffer[3] = {0};
	utester_assert_equal(buffer[0], 0);
	utester_assert_equal(buffer[1], 0);
	utester_assert_equal(buffer[2], 0);

	rl78misc_memset(buffer, 3, 3);
	utester_assert_equal(buffer[0], 3);
	utester_assert_equal(buffer[1], 3);
	utester_assert_equal(buffer[2], 3);
}

/**
 * @brief Construct a new utester define test for rl78misc_memcpy.
 */
utester_define_test(rl78misc_memcpy_test)
{
	uint8_t destination[3] = {0};
	utester_assert_equal(destination[0], 0);
	utester_assert_equal(destination[1], 0);
	utester_assert_equal(destination[2], 0);

	const uint8_t source[3] = {10, 20, 30};
	rl78misc_memcpy(destination, source, 3);
	utester_assert_equal(destination[0], 10);
	utester_assert_equal(destination[1], 20);
	utester_assert_equal(destination[2], 30);
}

/**
 * @brief Construct a new utester define test for rl78misc_memcmp.
 */
utester_define_test(rl78misc_memcmp_test)
{
	{
		const uint8_t buffer1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		const uint8_t buffer2[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		const int32_t result = rl78misc_memcmp(buffer1, buffer2, 10);
		utester_assert_equal(result, 0);
	}

	{
		const uint8_t buffer1[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
		const uint8_t buffer2[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		const int32_t result = rl78misc_memcmp(buffer1, buffer2, 10);
		utester_assert_not_equal(result, 0);
	}
}

/**
 * @brief Construct a new utester define test for rl78misc_strlen.
 */
utester_define_test(rl78misc_strlen_test)
{
	{
		const char_t buffer[] = "hello";
		const uint64_t length = rl78misc_strlen(buffer);
		utester_assert_equal(length, 5);
	}

	{
		const char_t buffer[] = "he\0llo";
		const uint64_t length = rl78misc_strlen(buffer);
		utester_assert_equal(length, 2);
	}
}

/**
 * @brief Construct a new utester define test for rl78misc_strcmp.
 */
utester_define_test(rl78misc_strcmp_test)
{
	{
		const char_t buffer1[] = "hello";
		const char_t buffer2[] = "hello";
		const int32_t result = rl78misc_strcmp(buffer1, buffer2);
		utester_assert_equal(result, 0);
	}

	{
		const char_t buffer1[] = "hello";
		const char_t buffer2[] = "world";
		const int32_t result = rl78misc_strcmp(buffer1, buffer2);
		utester_assert_not_equal(result, 0);
	}

	{
		const char_t buffer1[] = "hello";
		const char_t buffer2[] = "hellooo";
		const int32_t result = rl78misc_strcmp(buffer1, buffer2);
		utester_assert_not_equal(result, 0);
	}
}

/**
 * @brief Construct a new utester define test for rl78misc_strncmp.
 */
utester_define_test(rl78misc_strncmp_test)
{
	{
		const char_t buffer1[] = "hello";
		const char_t buffer2[] = "hello";
		const int32_t result = rl78misc_strncmp(buffer1, buffer2, 5);
		utester_assert_equal(result, 0);
	}

	{
		const char_t buffer1[] = "hello";
		const char_t buffer2[] = "hello";
		const int32_t result = rl78misc_strncmp(buffer1, buffer2, 3);
		utester_assert_equal(result, 0);
	}

	{
		const char_t buffer1[] = "hello";
		const char_t buffer2[] = "world";
		const int32_t result = rl78misc_strncmp(buffer1, buffer2, 3);
		utester_assert_not_equal(result, 0);
	}

	{
		const char_t buffer1[] = "hello";
		const char_t buffer2[] = "hellooo";
		const int32_t result = rl78misc_strncmp(buffer1, buffer2, 7);
		utester_assert_not_equal(result, 0);
	}
}

utester_run_suite(
	rl78misc_suite,
		&rl78misc_malloc_test,
		&rl78misc_realloc_test,
		&rl78misc_free_test,
		&rl78misc_memset_test,
		&rl78misc_memcpy_test,
		&rl78misc_memcmp_test,
		&rl78misc_strlen_test,
		&rl78misc_strcmp_test,
		&rl78misc_strncmp_test,
);
