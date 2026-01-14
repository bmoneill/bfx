#include "../bfx.h"
#include "common.h"
#include "ops.h"

void bfx_bf_init(bfx_t* bfx) { bfx_build_loops(bfx); }

void bfx_bf_run(bfx_t* bfx) {
    bfx_file_index_t idx;
    idx.idx                                     = 0;
    idx.line_idx                                = 0;
    idx.line                                    = 1;

    void (*ops[128])(bfx_t*, bfx_file_index_t*) = {
        [']'] = bfx_op_ret,     ['['] = bfx_op_call,    ['+'] = bfx_op_inc_t,
        ['-'] = bfx_op_dec_t,   ['>'] = bfx_op_inc_tp,  ['<'] = bfx_op_dec_tp,
        [','] = bfx_op_getchar, ['.'] = bfx_op_putchar,
    };

    while ((size_t) bfx->ip < bfx->program_len) {
        ops[(int) bfx->program[bfx->ip]](bfx, &idx);
        bfx->ip++;
    }
}
