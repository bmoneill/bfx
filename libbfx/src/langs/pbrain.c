/**
 * @file langs/pbrain.c
 *
 * This file contains the implementation of the P-Brain interpreter.
 */

#include "pbrain.h"
#include "bfx.h"
#include "ops.h"

#include <stdio.h>
#include <stdlib.h>

static void bfx_pbrain_populate_procedure(BFX*, BFX_FileIndex*);

/**
 * @brief Initialize the P-Brain interpreter
 * @param bfx Pointer to the already-allocated interpreter struct
 */
void bfx_pbrain_init(BFX* bfx) {
    bfx->lang_data       = malloc(sizeof(BFX_pbrainData));
    BFX_pbrainData* data = (BFX_pbrainData*) bfx->lang_data;
    data->procedures_len = 0;
    data->procedures     = NULL;
    bfx_build_loops(bfx);
}

/**
 * @brief Run the P-Brain interpreter
 * @param bfx Pointer to the interpreter struct
 */
void bfx_pbrain_run(BFX* bfx) {
    BFX_FileIndex idx;
    idx.idx                                = 0;
    idx.line_idx                           = 0;
    idx.line                               = 1;

    void (*ops[128])(BFX*, BFX_FileIndex*) = {
        [']'] = bfx_op_loop_end, ['['] = bfx_op_loop_start, ['+'] = bfx_op_inc_t,
        ['-'] = bfx_op_dec_t,    ['>'] = bfx_op_inc_tp,     ['<'] = bfx_op_dec_tp,
        [','] = bfx_op_getchar,  ['.'] = bfx_op_putchar,    ['('] = bfx_pbrain_populate_procedure,
        [':'] = bfx_op_call,     [')'] = bfx_op_ret,
    };

    while ((size_t) bfx->ip < bfx->program_len) {
        if (bfx->program[bfx->ip] == '\n') {
            idx.line++;
            idx.line_idx = 0;
        } else {
            idx.line_idx++;
        }

        int op = (int) bfx->program[bfx->ip];
        if (ops[op]) {
            ops[(int) bfx->program[bfx->ip]](bfx, &idx);
        }
        bfx->ip++;
    }
}

static void bfx_pbrain_populate_procedure(BFX* bf, BFX_FileIndex* idx) {
    size_t          i;
    BFX_pbrainData* data                              = (BFX_pbrainData*) bf->lang_data;

    data->procedures[data->procedures_len].start_idx  = bf->ip + 1;
    data->procedures[data->procedures_len].identifier = bf->tape[bf->tp];
    for (i = bf->ip; i < bf->program_len; i++) {
        if (bf->program[i] == ')') {
            data->procedures[data->procedures_len].end_idx = i;
            bf->ip                                         = i + 1;
            data->procedures_len++;
            return;
        }
    }
    BFX_ERROR("Expected ')'");
}
