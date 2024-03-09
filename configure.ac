
# Init autoconf
AC_INIT([rl78emu], [0.1.0], [https://github.com/joba14/rl78emu/issues], [rl78emu], [https://github.com/joba14/rl78emu])

# Minimum accepted autoconf version
AC_PREREQ([2.69])  # todo: roll back to 2.71

# Source directory (with sanity check file)
AC_CONFIG_SRCDIR([./makefile.am])

# Auxiliary files directory
AC_CONFIG_AUX_DIR([./aux])

# Initializing automake tool
AM_INIT_AUTOMAKE([foreign subdir-objects])

# Setup output targets
AC_CONFIG_FILES([makefile ./include/rl78emu/version.h:version.h.in])

# Initializing linking tool
LT_INIT

# Checks for header files
AC_CHECK_HEADER([assert.h], [],
	AC_MSG_ERROR([The 'assert.h' header was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_HEADER([stdlib.h], [],
	AC_MSG_ERROR([The 'stdlib.h' header was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_HEADER([stddef.h], [],
	AC_MSG_ERROR([The 'stddef.h' header was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_HEADER([stdarg.h], [],
	AC_MSG_ERROR([The 'stdarg.h' header was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_HEADER([stdint.h], [],
	AC_MSG_ERROR([The 'stdint.h' header was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_HEADER([string.h], [],
	AC_MSG_ERROR([The 'string.h' header was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_HEADER([limits.h], [],
	AC_MSG_ERROR([The 'limits.h' header was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_HEADER([float.h], [],
	AC_MSG_ERROR([The 'float.h' header was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_HEADER([stdio.h], [],
	AC_MSG_ERROR([The 'stdio.h' header was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_HEADER([ctype.h], [],
	AC_MSG_ERROR([The 'ctype.h' header was not found! Cannot proceed with the build process without it...])
)

# Checks for typedefs, structures, and compiler characteristics
AC_C_STRINGIZE
AC_C_INLINE
AC_C_CONST
AC_C_RESTRICT
AC_C_CHAR_UNSIGNED
AC_C_VOLATILE
AC_C_VARARRAYS

# Checks for types
AC_CHECK_TYPE([char], [],
	[AC_MSG_ERROR([The 'char' type was not found! Cannot proceed with the build process without it...])]
)

AC_CHECK_TYPE([signed char], [],
	[AC_MSG_ERROR([The 'signed char' type was not found! Cannot proceed with the build process without it...])]
)

AC_CHECK_TYPE([unsigned char], [],
	[AC_MSG_ERROR([The 'unsigned char' type was not found! Cannot proceed with the build process without it...])]
)

AC_CHECK_TYPE([signed short], [],
	[AC_MSG_ERROR([The 'signed short' type was not found! Cannot proceed with the build process without it...])]
)

AC_CHECK_TYPE([unsigned short], [],
	[AC_MSG_ERROR([The 'unsigned short' type was not found! Cannot proceed with the build process without it...])]
)

AC_CHECK_TYPE([signed int], [],
	[AC_MSG_ERROR([The 'signed int' type was not found! Cannot proceed with the build process without it...])]
)

AC_CHECK_TYPE([unsigned int], [],
	[AC_MSG_ERROR([The 'unsigned int' type was not found! Cannot proceed with the build process without it...])]
)

AC_CHECK_TYPE([signed long long], [],
	[AC_MSG_ERROR([The 'signed long long' type was not found! Cannot proceed with the build process without it...])]
)

AC_CHECK_TYPE([unsigned long long], [],
	[AC_MSG_ERROR([The 'unsigned long long' type was not found! Cannot proceed with the build process without it...])]
)

AC_CHECK_TYPE([float], [],
	[AC_MSG_ERROR([The 'float' type was not found! Cannot proceed with the build process without it...])]
)

AC_CHECK_TYPE([double], [],
	[AC_MSG_ERROR([The 'double' type was not found! Cannot proceed with the build process without it...])]
)

AC_CHECK_TYPE([long double], [],
	[AC_MSG_ERROR([The 'long double' type was not found! Cannot proceed with the build process without it...])]
)

# Checks for library functions
AC_CHECK_FUNC([malloc], [],
	AC_MSG_ERROR([The 'malloc' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([realloc], [],
	AC_MSG_ERROR([The 'realloc' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([free], [],
	AC_MSG_ERROR([The 'free' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([printf], [],
	AC_MSG_ERROR([The 'printf' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([fprintf], [],
	AC_MSG_ERROR([The 'fprintf' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([vfprintf], [],
	AC_MSG_ERROR([The 'vfprintf' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([sscanf], [],
	AC_MSG_ERROR([The 'sscanf' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([memcpy], [],
	AC_MSG_ERROR([The 'memcpy' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([memcmp], [],
	AC_MSG_ERROR([The 'memcmp' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([memset], [],
	AC_MSG_ERROR([The 'memset' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([strncmp], [],
	AC_MSG_ERROR([The 'strncmp' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([strlen], [],
	AC_MSG_ERROR([The 'strlen' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([fopen], [],
	AC_MSG_ERROR([The 'fopen' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([fclose], [],
	AC_MSG_ERROR([The 'fclose' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([fgetc], [],
	AC_MSG_ERROR([The 'fgetc' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([fgets], [],
	AC_MSG_ERROR([The 'fgets' function was not found! Cannot proceed with the build process without it...])
)

AC_CHECK_FUNC([fseek], [],
	AC_MSG_ERROR([The 'fseek' function was not found! Cannot proceed with the build process without it...])
)

# Find C compiler
AC_PROG_CC([gcc cc])

# Set language to C
AC_LANG(C)

# Generate output
AC_OUTPUT

# Printing out the configuration
echo ""
echo "Configuration Parameters:"
echo "target............... ${with_target}"
echo "AR_FLAGS............. ${AR_FLAGS}"
echo "CC................... ${CC}"
echo "CFLAGS............... ${CFLAGS}"
echo "CPPFLAGS............. ${CPPFLAGS}"
echo "HOST................. ${HOST}"
echo "HOSTNAME............. ${HOSTNAME}"
echo "LDFLAGS.............. ${LDFLAGS}"
echo "host................. ${host}"
echo "install prefix ...... ${prefix}"
echo ""
