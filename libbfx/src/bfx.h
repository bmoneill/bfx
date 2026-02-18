/**
  * @file bfx.c
  * @brief Public definitions for main logic for the brainfuck-like language interpreter/compiler.
  */
#ifndef BFX_H
#define BFX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BFX_DEFAULT_INPUT_MAX
/**
 * @brief Default maximum input size for the interpreter.
 */
#define BFX_DEFAULT_INPUT_MAX 1024
#endif

#ifndef BFX_DEFAULT_TAPE_SIZE
/**
 * @brief Default tape size for the interpreter.
 */
#define BFX_DEFAULT_TAPE_SIZE 30000
#endif

#ifndef BFX_INITIAL_LOOP_SIZE
/**
 * @brief Default maximum loop count for the interpreter (scaled anyways if higher)
 */
#define BFX_INITIAL_LOOP_SIZE 2048
#endif

#ifndef BFX_PROCEDURE_SIZE
/**
 * @brief Default maximum procedure count for interpreter
 */
#define BFX_PROCEDURE_SIZE 128
#endif

#ifndef BFX_VERSION
/**
 * @brief Version of the libbfx library
 */
#define BFX_VERSION "unknown"
#endif

#ifndef BFX_DEFAULT_LANG
/**
 * @brief Default language for the interpreter/compiler
 */
#define BFX_DEFAULT_LANG BFX_LANG_BRAINFUCK
#endif

#ifndef BFX_DEFAULT_EOF_BEHAVIOR
/**
 * @brief Default EOF behavior for the interpreter
 */
#define BFX_DEFAULT_EOF_BEHAVIOR BFX_EOF_BEHAVIOR_ZERO
#endif

/**
 * @brief Behaviors for the interpreter when an EOF is encountered in input
 */
typedef enum {
    BFX_EOF_BEHAVIOR_ZERO, //!< Set the current cell to zero
    BFX_EOF_BEHAVIOR_DECREMENT, //!< Decrement the current cell by 1
    BFX_EOF_BEHAVIOR_UNCHANGED //!< Leave the current cell unchanged
} BFX_EOFBehavior;

#define BFX_FLAG_DEBUG                        1 //!< Interpreter: Debug mode
#define BFX_FLAG_REPL                         2 //!< Interpreter: REPL mode
#define BFX_FLAG_DISABLE_SPECIAL_INSTRUCTIONS 4 //!< Interpreter: Disable special instructions
#define BFX_FLAG_LANG_DATA_FLAGS              8 //!< lang_data contains flags for the given language
#define BFX_FLAG_GRAPHICS                     16 //!< Interpreter: Enable graphics mode
#define BFX_FLAG_SEPARATE_INPUT_AND_SOURCE                                                         \
    32 //!< Interpreter: Use input in source file separated by '!'
#define BFX_FLAG_DEGREES 64 //!< Interpreter (Grin): Use degrees instead of radians

/**
 * @brief Language options for the interpreter/compiler
 */
typedef enum {
    BFX_LANG_UNKNOWN, //!< Unknown language
    BFX_LANG_brainfuck, //!< brainfuck language
    BFX_LANG_brainfork, //!< brainfork language
    BFX_LANG_pbrain, //!< pbrain language
    BFX_LANG_grin, //!< Grin language
    BFX_LANG_weave //!< Weave language
} BFX_Language;

typedef enum {
    BFX_SUCCESS = 0, //!< No error
    BFX_STACK_OVERFLOW_ERROR, //!< Stack overflow error
    BFX_STACK_UNDERFLOW_ERROR, //!< Stack underflow error
    BFX_SYNTAX_ERROR, //!< Syntax error
    BFX_OUT_OF_MEMORY_ERROR, //!< Out of memory error
    BFX_RUNTIME_ERROR, //!< Runtime error
} BFX_Error;

/**
 * @brief Error handling macro for the interpreter
 */
#define BFX_ERROR(s) fprintf(stderr, "libbfx: Error: %s\n", s);

/**
 * @brief Check if the interpreter is in debug mode (arg is of type BFX)
 */
#define BFX_IN_DEBUG_MODE(b) ((b).flags & BFX_FLAG_DEBUG)

/**
 * @brief Check if the interpreter is in REPL mode (arg is of type BFX)
 */
#define BFX_IN_REPL_MODE(b) ((b).flags & BFX_FLAG_REPL)

/**
 * @brief Check if the interpreter has special instructions enabled (arg is of type BFX)
 */
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
 * @struct BFX
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

void      bfx_free(BFX*);
BFX_Error bfx_run_file(const char*, BFX*);
BFX_Error bfx_run_repl(BFX*);

#endif
