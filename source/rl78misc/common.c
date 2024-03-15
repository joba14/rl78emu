
/**
 * @file common.c
 * 
 * @copyright This file is a part of the "rl78emu" project and is licensed, and
 * distributed under "rl78emu gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-03-03
 */

#include "rl78misc/common.h"
#include "rl78misc/debug.h"
#include "rl78misc/logger.h"

#include <stdlib.h>
#include <memory.h>
#include <string.h>

void* rl78misc_malloc(const uint64_t size)
{
	rl78misc_debug_assert(size > 0);
	void* const pointer = (void* const)malloc(size);

	if (NULL == pointer)
	{
		rl78misc_logger_error("internal failure -- failed to allocate memory.");
		rl78misc_exit(-1);
	}

	return pointer;
}

void* rl78misc_realloc(void* pointer, const uint64_t size)
{
	rl78misc_debug_assert(size > 0);
	pointer = (void*)realloc(pointer, size);

	if (NULL == pointer)
	{
		rl78misc_logger_error("internal failure -- failed to reallocate memory.");
		rl78misc_exit(-1);
	}

	return pointer;
}

void* rl78misc_free(const void* pointer)
{
	free((void*)pointer);
	return NULL;
}

void rl78misc_exit(const int32_t code)
{
	exit(code);
}

void rl78misc_memset(void* const pointer, const uint8_t value, const uint64_t length)
{
	rl78misc_debug_assert(pointer != NULL);
	(void)memset((void*)pointer, value, length);
}

void rl78misc_memcpy(void* const destination, const void* const source, const uint64_t length)
{
	rl78misc_debug_assert(destination != NULL);
	rl78misc_debug_assert(source != NULL);
	rl78misc_debug_assert(length > 0);
	(void)memcpy((void*)destination, (const void*)source, length);
}

int32_t rl78misc_memcmp(const uint8_t* const left, const uint8_t* const right, const uint64_t length)
{
	return (int32_t)(memcmp(left, right, length));
}

uint64_t rl78misc_strlen(const char_t* const cstring)
{
	rl78misc_debug_assert(cstring != NULL);
	return (uint64_t)strlen(cstring);
}

int32_t rl78misc_strcmp(const char_t* const left, const char_t* const right)
{
	rl78misc_debug_assert(left != NULL);
	rl78misc_debug_assert(right != NULL);
	return strcmp((const char_t*)left, (const char_t*)right);
}

int32_t rl78misc_strncmp(const char_t* const left, const char_t* const right, const uint64_t length)
{
	rl78misc_debug_assert(left != NULL);
	rl78misc_debug_assert(right != NULL);
	return strncmp((const char_t*)left, (const char_t*)right, (size_t)length);
}
