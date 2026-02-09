/**
 * @file langs/pbrain.c
 *
 * This file contains the implementation of the P-Brain interpreter.
 */

#include "pbrain.h"
#include "bfx.h"
#include "ops.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Initialize the P-Brain interpreter
 *
 * @param bfx Pointer to the already-allocated interpreter struct
 */
void bfx_pbrain_init(BFX* bfx) {
    bfx->lang_data       = calloc(1, sizeof(BFX_pbrainData));
    BFX_pbrainData* data = (BFX_pbrainData*) bfx->lang_data;
    data->procedures_len = 0;
    data->procedures     = malloc(sizeof(BFX_pbrainProcedure) * BFX_PBRAIN_MAX_PROCEDURES);
    data->stack          = malloc(sizeof(size_t) * BFX_INITIAL_LOOP_SIZE);
    bfx_build_loops(bfx);
}

/**
 * @brief Populate the procedure table with the given procedure.
 *
 * @param bf Pointer to the interpreter struct
 * @param idx Pointer to the file index struct
 */
void bfx_pbrain_populate_procedure(BFX* bf, BFX_FileIndex* idx) {
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
        [']'] = bfx_op_loop_end, ['['] = bfx_op_loop_start, ['+'] = bfx_op_inc_t,
        ['-'] = bfx_op_dec_t,    ['>'] = bfx_op_inc_tp,     ['<'] = bfx_op_dec_tp,
        [','] = bfx_op_getchar,  ['.'] = bfx_op_putchar,    ['('] = bfx_pbrain_populate_procedure,
        [':'] = bfx_op_call,     [')'] = bfx_op_ret,
    };

    bfx_parse_ops(bfx, ops);

    // Free language-specific data
    BFX_pbrainData* data = (BFX_pbrainData*) bfx->lang_data;
    free(data->procedures);
    free(data->stack);
    free(data);
}
