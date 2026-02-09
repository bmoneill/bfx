/**
 * @file langs/grin.c
 *
 * This file contains the implementation of the Grin interpreter.
 */

#include "grin.h"
#include "bfx.h"
#include "ops.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Initialize the Grin interpreter
 *
 * @param bfx Pointer to the already-allocated interpreter struct
 */
void bfx_grin_init(BFX* bfx) {
    if (bfx->tape) {
        free(bfx->tape);
    }

    bfx->lang_data     = calloc(1, sizeof(BFX_GrinData));
    BFX_GrinData* data = (BFX_GrinData*) bfx->lang_data;
    data->unit         = bfx->flags & BFX_FLAG_DEGREES ? BFX_GRIN_DEG : BFX_GRIN_RAD;
    data->precision    = BFX_GRIN_DEFAULT_PRECISION;
    bfx_build_loops(bfx);
}

/**
 * @brief Run the Grin interpreter
 *
 * This function initializes and runs the Grin interpreter.
 * It frees all language-specific data.
 *
 * @param bfx Pointer to the interpreter struct
 */
void bfx_pbrain_run(BFX* bfx) {
    void (*ops[128])(BFX*, BFX_FileIndex*) = { ['<']  = bfx_op_dec_tp,
                                               ['>']  = bfx_op_inc_tp,
                                               ['.']  = bfx_op_putchar_ascii,
                                               [':']  = bfx_op_putchar_number,
                                               [',']  = bfx_op_getchar_ascii,
                                               [';']  = bfx_op_getchar_number,
                                               ['\''] = bfx_op_putchar_register_ascii,
                                               ['"']  = bfx_op_putchar_register_number,
                                               ['+']  = bfx_op_add,
                                               ['-']  = bfx_op_sub,
                                               ['*']  = bfx_op_mul,
                                               ['/']  = bfx_op_div,
                                               ['^']  = bfx_op_exp,
                                               ['$']  = bfx_op_store,
                                               ['\\'] = bfx_op_load,
                                               ['~']  = bfx_op_swap,
                                               ['=']  = bfx_op_zero,
                                               ['_']  = bfx_op_zero_ptr,
                                               ['%']  = bfx_op_mod,
                                               ['r']  = bfx_op_round,
                                               ['&']  = bfx_op_nand,
                                               ['|']  = bfx_op_or,
                                               ['!']  = bfx_op_not,
                                               ['@']  = bfx_op_negate,
                                               ['[']  = bfx_op_loop_start,
                                               [']']  = bfx_op_loop_end,
                                               ['?']  = bfx_op_simplify,
                                               ['}']  = bfx_op_inc_t,
                                               ['{']  = bfx_op_dec_t,
                                               ['(']  = bfx_op_print,
                                               [')']  = bfx_op_newline,
                                               ['s']  = bfx_op_sin,
                                               ['c']  = bfx_op_cos,
                                               ['t']  = bfx_op_tan,
                                               ['S']  = bfx_op_arcsin,
                                               ['C']  = bfx_op_arccos,
                                               ['T']  = bfx_op_arctan,
                                               ['1']  = bfx_op_inv,
                                               ['2']  = bfx_op_mod2,
                                               ['e']  = bfx_op_e,
                                               ['p']  = bfx_op_pi,
                                               ['l']  = bfx_op_ln,
                                               ['m']  = bfx_op_mean,
                                               ['q']  = bfx_op_sqrt,
                                               ['L']  = bfx_op_log,
                                               ['j']  = bfx_op_jump,
                                               ['D']  = bfx_op_toggle_deg_rad,
                                               ['`']  = bfx_op_exit };

    bfx_parse_ops(bfx, ops);

    // Free language-specific data
    BFX_GrinData* data = (BFX_GrinData*) bfx->lang_data;
    free(data->tape);
    free(data);
}
