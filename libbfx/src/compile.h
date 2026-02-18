/**
 * @file compile.h
 *
 * This file contains the public definitions for the Brainfuck-like language compiler.
 */

#ifndef BFX_COMPILE_H
#define BFX_COMPILE_H

#include "bfx.h"

#ifndef BFX_DEFAULT_COMPILER
/**
 * @brief Default compiler to use when compiling generated C code.
 */
#define BFX_DEFAULT_COMPILER "gcc"
#endif

#ifndef BFX_DEFAULT_COMPILE_FLAGS
/**
 * @brief Default compile flags to use when compiling generated C code.
 */
#define BFX_DEFAULT_COMPILE_FLAGS "-O3 -s -ffast-math -static"
#endif

#ifndef BFX_COMPILER_LDFLAGS
/**
 * @brief Default linker flags to use when compiling generated C code.
 */
#define BFX_COMPILER_LDFLAGS "-lbfx -lm"
#endif

#ifndef BFX_COMPILE_HEAD
/**
 * @brief Head of the compiled program
 */
#define BFX_COMPILE_HEAD "#include <bfx/interpret.h>\nint main(void) {BFX bfx;"
#endif

#ifndef BFX_TMP_FILE_PATH
/**
 * @brief Path to the temporary file used for compilation
 */
#define BFX_TMP_FILE_PATH "/tmp/bfx.c"
#endif

void bfx_compile(const char*, const char*, BFX*);

#endif
