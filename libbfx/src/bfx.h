/**
 * @file bfx.h
 * @brief brainfuck interpreter header
 * @author Ben O'Neill <ben@oneill.sh>
 *
 * @copyright Copyright (c) 2022-2025 Ben O'Neill <ben@oneill.sh>.
 * This work is released under the terms of the MIT License. See
 * LICENSE.
 */

#ifndef BFX_H
#define BFX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef BFX_DEFAULT_COMPILER
#define BFX_DEFAULT_COMPILER "gcc"
#endif

#ifndef BFX_DEFAULT_COMPILE_FLAGS
#define BFX_DEFAULT_COMPILE_FLAGS "-O3 -s -ffast-math"
#endif

#ifndef BFX_DEFAULT_INPUT_MAX
#define BFX_DEFAULT_INPUT_MAX 1024
#endif

#ifndef BFX_DEFAULT_TAPE_SIZE
#define BFX_DEFAULT_TAPE_SIZE 30000
#endif

#ifndef BFX_INITIAL_LOOP_SIZE
#define BFX_INITIAL_LOOP_SIZE 2048
#endif

#ifndef BFX_PROCEDURE_SIZE
#define BFX_PROCEDURE_SIZE 1024
#endif

#ifndef BFX_VERSION
#define BFX_VERSION "unknown"
#endif

#define BFX_EOF_BEHAVIOR_ZERO      0
#define BFX_EOF_BEHAVIOR_DECREMENT 1
#define BFX_EOF_BEHAVIOR_UNCHANGED 2

#define BFX_FLAG_DEBUG                        1
#define BFX_FLAG_REPL                         2
#define BFX_FLAG_DISABLE_SPECIAL_INSTRUCTIONS 4
#define BFX_FLAG_ONLY_GENERATE_C_SOURCE       8
#define BFX_FLAG_GRAPHICS                     16
#define BFX_FLAG_SEPARATE_INPUT_AND_SOURCE    32

typedef enum {
    BFX_LANG_BRAINFUCK,
    BFX_LANG_BRAINFORK,
    BFX_LANG_PBRAIN,
    BFX_LANG_GRIN,
    BFX_LANG_WEAVE
} BFX_Language;

#define BFX_DEFAULT_LANG BFX_LANG_BRAINFUCK

#define BFX_DEFAULT_EOF_BEHAVIOR BFX_EOF_BEHAVIOR_ZERO

#define BFX_ERROR(s)                                                                               \
    fprintf(stderr, "libbfx: Error: %s\n", s);                                                     \
    exit(EXIT_FAILURE);

#define BFX_IN_DEBUG_MODE(b)                ((b).flags & BFX_FLAG_DEBUG)
#define BFX_IN_REPL_MODE(b)                 ((b).flags & BFX_FLAG_REPL)
#define BFX_SPECIAL_INSTRUCTIONS_ENABLED(b) (!((b).flags & BFX_FLAG_DISABLE_SPECIAL_INSTRUCTIONS))

/**
 * @brief Structure to represent an index in a file (or user input).
 */
typedef struct {
    int idx; //!< Index within the file or user input
    int line; //!< Line number in the file or user input.
    int line_idx; //!< Index within the line.
} BFX_FileIndex;

/**
 *  @brief Structure to represent a loop in the brainfuck program.
 *   This structure holds the start and end indices of a loop,
 *   allowing the interpreter to efficiently jump between matching
 *   brackets during execution.
 *   @note The `start` field represents the index of the opening bracket '[',
 *         and the `end` field represents the index of the closing bracket ']'.
 */
typedef struct {
    BFX_FileIndex start; //!< Start index of block
    BFX_FileIndex end; //!< End index of block
} BFX_Block;

/**
 * @struct bfx_t
 * @brief Structure to store generic data for a brainfuck-like esolang interpreter.
 */
typedef struct {
    uint16_t     flags; //!< Flags to control compilation and execution behavior.
    bool         receiving; //!< Indicates whether the interpreter is receiving input.
    char*        program; //!< Pointer to the program string.
    size_t       program_len; //!< Length of the program string (not including the input buffer).
    size_t       program_size; //!< Size of the program string.
    size_t       input_start; //!< Start index of the input buffer in the program string.
    size_t       input_ptr; //!< Current index of the input buffer in the program string.
    size_t       input_len; //!< Length of the input buffer in the program string.
    uint8_t*     tape; //!< Pointer to the tape array.
    size_t       tape_size; //!< Size of the tape array.
    int          ip; //!< Instruction pointer.
    int          tp; //!< Tape pointer.
    int          tp_max; //!< Maximum tape pointer value since execution started.
    BFX_Block*   loops; //!< Pointer to the loop block array.
    size_t       loops_len; //!< Length of the loop array.
    size_t       loops_size; //!< Size of the loop array.
    size_t       input_max; //!< Maximum size of the input buffer.
    int          eof_behavior; //!< Behavior when encountering EOF.
    BFX_Language lang; //!< Language identifier for the brainfuck-like esolang.
    void*        lang_data; //!< Pointer to language-specific data structure.
} BFX;

void bfx_build_loops(BFX*);
void bfx_free(BFX*);
void bfx_reset(BFX*);
void bfx_run_file(const char*, BFX*);
void bfx_run_repl(BFX*);

#endif
