/**
 * @file langs/pbrain.c
 *
 * This file contains the implementation of the P-Brain interpreter.
 */

#include "pbrain.h"

#include "../bfx.h"
#include "brainfuck.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Initialize the P-Brain interpreter
 *
 * @param bfx Pointer to the already-allocated interpreter struct
 */
void bfx_pbrain_init(BFX* bfx) {
    bfx->lang_data       = calloc(1, sizeof(BFX_PBrainData));
    BFX_PBrainData* data = (BFX_PBrainData*) bfx->lang_data;
    data->procedures_len = 0;
    data->procedures     = malloc(sizeof(BFX_PBrainProcedure) * BFX_PBRAIN_MAX_PROCEDURES);
    data->stack          = malloc(sizeof(size_t) * BFX_INITIAL_LOOP_SIZE);
    bfx_build_loops(bfx);
}

/**
 * @brief Run the P-Brain interpreter
 *
 * This function initializes and runs the P-Brain interpreter.
 * It frees all language-specific data.
 *
 * @param bfx Pointer to the interpreter struct
 */
void bfx_pbrain_run(BFX* bfx) {
    void (*ops[128])(BFX*, BFX_FileIndex*) = {
        [']'] = bfx_op_brainfuck_loop_end,
        ['['] = bfx_op_brainfuck_loop_start,
        ['+'] = bfx_op_brainfuck_inc_t,
        ['-'] = bfx_op_brainfuck_dec_t,
        ['>'] = bfx_op_brainfuck_inc_tp,
        ['<'] = bfx_op_brainfuck_dec_tp,
        [','] = bfx_op_brainfuck_getchar,
        ['.'] = bfx_op_brainfuck_putchar,
        ['('] = bfx_op_pbrain_start_procedure,
        [':'] = bfx_op_pbrain_call,
        [')'] = bfx_op_pbrain_ret,
    };

    bfx_parse_ops(bfx, ops);

    // Free language-specific data
    BFX_PBrainData* data = (BFX_PBrainData*) bfx->lang_data;
    free(data->procedures);
    free(data->stack);
    free(data);
}

/**
 * @brief Begin procedure definition (pbrain).
 *
 * @param bf Pointer to the interpreter struct
 * @param idx Pointer to the file index struct
 */
void bfx_op_pbrain_start_procedure(BFX* bfx, BFX_FileIndex* idx) {
    size_t          i;
    BFX_PBrainData* data                              = (BFX_PBrainData*) bfx->lang_data;

    data->procedures[data->procedures_len].start_idx  = bfx->ip + 1;
    data->procedures[data->procedures_len].identifier = bfx->tape[bfx->tp];
    for (i = bfx->ip; i < bfx->program_len; i++) {
        if (bfx->program[i] == ')') {
            data->procedures[data->procedures_len].end_idx = i;
            bfx->ip                                        = i + 1;
            data->procedures_len++;
            return;
        }
    }
    BFX_ERROR("Expected ')'");
}

/**
 * @brief Call a procedure by its identifier (pbrain).
 */
void bfx_op_pbrain_call(BFX* bfx, BFX_FileIndex* index) {
    BFX_PBrainData* data = (BFX_PBrainData*) bfx->lang_data;
    for (size_t i = 0; i < data->procedures_len; i++) {
        if (data->procedures[i].identifier == bfx->tape[bfx->tp]) {
            data->stack_top++;
            data->stack[data->stack_top] = bfx->ip;
            bfx->ip                      = data->procedures[i].start_idx;
            return;
        }
    }
}

/**
 * @brief Return from a procedure (pbrain).
 */
void bfx_op_pbrain_ret(BFX* bfx, BFX_FileIndex* index) {
    BFX_PBrainData* data = (BFX_PBrainData*) bfx->lang_data;
    if (data->stack_top > 0) {
        bfx->ip = data->stack[data->stack_top];
        data->stack_top--;
    }
}
