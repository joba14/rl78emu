
# Minimum cmake version required
cmake_minimum_required(VERSION 3.5)

# Project name and programming language definition
project(rl78emu LANGUAGES C)

# C standard for the project
set(CMAKE_C_STANDARD 11)

# Source files of the project
set(SOURCES
	./source/rl78misc/common.c
	./source/rl78misc/debug.c
	./source/rl78misc/logger.c
	./source/rl78core/mem.c
	./source/rl78core/cpu.c
	./source/rl78cli/config.c
	./source/rl78cli/main.c
)

set(PACKAGE_VERSION "1.0.0")
configure_file(./version.h.in ${CMAKE_BINARY_DIR}/include/rl78emu/version.h)

# Project executable definition
add_executable(${PROJECT_NAME} ${SOURCES})

# Compile options of the project
target_compile_options(${PROJECT_NAME}
	PRIVATE
		-Wall
		-Wextra
		-Wpedantic
		-Werror
		-Wshadow
		-Wimplicit
		-Wreturn-type
		-Wunknown-pragmas
		-Wunused-variable
		-Wunused-function
		-Wmissing-prototypes
		-Wstrict-prototypes
		-Wconversion
		-Wsign-conversion
		-Wunreachable-code
)

# Include directories of the project
target_include_directories(${PROJECT_NAME}
	PUBLIC
		./include
		./build/include
	PRIVATE
)

# Link libraries for the target
target_link_libraries(${PROJECT_NAME}
	PUBLIC
	PRIVATE
)

# Compile definitions of the project
target_compile_definitions(${PROJECT_NAME}
	PUBLIC
	PRIVATE
)
