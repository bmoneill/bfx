#include "pbrain.h"
#include "bfx.h"
#include "ops.h"

#include <stdlib.h>

static void bfx_pbrain_populate_procedure(bfx_t*, bfx_file_index_t*);

void        bfx_pbrain_init(bfx_t* bfx) {
    bfx->lang_data          = malloc(sizeof(bfx_pbrain_data_t));
    bfx_pbrain_data_t* data = (bfx_pbrain_data_t*) bfx->lang_data;
    data->procedures_len    = 0;
    data->procedures        = NULL;
    bfx_build_loops(bfx);
}

void bfx_pbrain_run(bfx_t* bfx) {
    bfx_file_index_t idx;
    idx.idx                                     = 0;
    idx.line_idx                                = 0;
    idx.line                                    = 1;

    void (*ops[128])(bfx_t*, bfx_file_index_t*) = {
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

static void bfx_pbrain_populate_procedure(bfx_t* bf, bfx_file_index_t* idx) {
    size_t             i;
    bfx_pbrain_data_t* data                           = (bfx_pbrain_data_t*) bf->lang_data;

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
}
