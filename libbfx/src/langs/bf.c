/**
 * @file langs/bf.c
 *
 * This file contains the implementation of the Brainfuck interpreter.
 */

#include "../bfx.h"
#include "ops.h"
#include "util.h"

/**
 * @brief Initialize the Brainfuck interpreter
 * @param bfx Pointer to the already-allocated interpreter struct
 */
void bfx_bf_init(BFX* bfx) { bfx_build_loops(bfx); }

/**
 * @brief Run the Brainfuck interpreter
 * @param bfx Pointer to the interpreter struct
 */
void bfx_bf_run(BFX* bfx) {
    void (*ops[128])(BFX*, BFX_FileIndex*) = {
        [']'] = bfx_op_loop_end, ['['] = bfx_op_loop_start, ['+'] = bfx_op_inc_t,
        ['-'] = bfx_op_dec_t,    ['>'] = bfx_op_inc_tp,     ['<'] = bfx_op_dec_tp,
        [','] = bfx_op_getchar,  ['.'] = bfx_op_putchar,
    };
    bfx_parse_ops(bfx, ops);
}
