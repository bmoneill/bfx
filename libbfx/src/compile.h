/**
 * @file compile.h
 *
 * This file contains the public definitions for the Brainfuck-like language compiler.
 */

#ifndef BFX_COMPILE_H
#define BFX_COMPILE_H

#include "bfx.h"

#ifndef BFX_COMPILE_HEAD
/**
 * @brief Head of the compiled program
 */
#define BFX_COMPILE_HEAD "#include <bfx/bfx.h>\nint main(void) {BFX bfx; char* program;"
#endif

#ifndef BFX_TMP_FILE_PATH
/**
 * @brief Path to the temporary file used for compilation
 */
#define BFX_TMP_FILE_PATH "/tmp/bfx.c"
#endif

void bfx_compile(const char*, const char*, BFX*);

#endif
