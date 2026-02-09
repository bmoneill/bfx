/**
 * @file langs/ops.h
 *
 * This file contains public definitions for all ops for the supported languages.
 */

#ifndef BFX_OPS_H
#define BFX_OPS_H

#include "../bfx.h"

/* brainfuck */
void bfx_op_inc_tp(BFX*, BFX_FileIndex*);
void bfx_op_dec_tp(BFX*, BFX_FileIndex*);
void bfx_op_inc_t(BFX*, BFX_FileIndex*);
void bfx_op_dec_t(BFX*, BFX_FileIndex*);
void bfx_op_loop_start(BFX*, BFX_FileIndex*);
void bfx_op_loop_end(BFX*, BFX_FileIndex*);
void bfx_op_getchar(BFX*, BFX_FileIndex*);
void bfx_op_putchar(BFX*, BFX_FileIndex*);

/* pbrain */
void bfx_op_call(BFX*, BFX_FileIndex*);
void bfx_op_ret(BFX*, BFX_FileIndex*);

/* weave */
void bfx_op_toggle(BFX*, BFX_FileIndex*);

/* grin */
void bfx_op_putchar_ascii(BFX*, BFX_FileIndex*);
void bfx_op_putchar_number(BFX*, BFX_FileIndex*);
void bfx_op_getchar_ascii(BFX*, BFX_FileIndex*);
void bfx_op_getchar_number(BFX*, BFX_FileIndex*);
void bfx_op_putchar_register_ascii(BFX*, BFX_FileIndex*);
void bfx_op_putchar_register_number(BFX*, BFX_FileIndex*);
void bfx_op_add(BFX*, BFX_FileIndex*);
void bfx_op_sub(BFX*, BFX_FileIndex*);
void bfx_op_mul(BFX*, BFX_FileIndex*);
void bfx_op_div(BFX*, BFX_FileIndex*);
void bfx_op_exp(BFX*, BFX_FileIndex*);
void bfx_op_store(BFX*, BFX_FileIndex*);
void bfx_op_load(BFX*, BFX_FileIndex*);
void bfx_op_swap(BFX*, BFX_FileIndex*);
void bfx_op_zero(BFX*, BFX_FileIndex*);
void bfx_op_zero_ptr(BFX*, BFX_FileIndex*);
void bfx_op_mod(BFX*, BFX_FileIndex*);
void bfx_op_round(BFX*, BFX_FileIndex*);
void bfx_op_nand(BFX*, BFX_FileIndex*);
void bfx_op_or(BFX*, BFX_FileIndex*);
void bfx_op_not(BFX*, BFX_FileIndex*);
void bfx_op_negate(BFX*, BFX_FileIndex*);
void bfx_op_simplify(BFX*, BFX_FileIndex*);
void bfx_op_print(BFX*, BFX_FileIndex*);
void bfx_op_newline(BFX*, BFX_FileIndex*);
void bfx_op_sin(BFX*, BFX_FileIndex*);
void bfx_op_cos(BFX*, BFX_FileIndex*);
void bfx_op_tan(BFX*, BFX_FileIndex*);
void bfx_op_arcsin(BFX*, BFX_FileIndex*);
void bfx_op_arccos(BFX*, BFX_FileIndex*);
void bfx_op_arctan(BFX*, BFX_FileIndex*);
void bfx_op_inv(BFX*, BFX_FileIndex*);
void bfx_op_mod2(BFX*, BFX_FileIndex*);
void bfx_op_e(BFX*, BFX_FileIndex*);
void bfx_op_pi(BFX*, BFX_FileIndex*);
void bfx_op_ln(BFX*, BFX_FileIndex*);
void bfx_op_mean(BFX*, BFX_FileIndex*);
void bfx_op_sqrt(BFX*, BFX_FileIndex*);
void bfx_op_log(BFX*, BFX_FileIndex*);
void bfx_op_jump(BFX*, BFX_FileIndex*);
void bfx_op_toggle_deg_rad(BFX*, BFX_FileIndex*);
void bfx_op_exit(BFX*, BFX_FileIndex*);

#endif
