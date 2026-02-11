/**
 * @file langs/grin.h
 *
 * This file contains function definitions related to interpreting the Grin language.
 */
#ifndef BFX_LANGS_GRIN_H
#define BFX_LANGS_GRIN_H

#include "bfx.h"

/**
 * @brief Unit of measurement for Grin language.
 */
typedef enum {
    BFX_GRIN_DEG = 0, //!< Degrees
    BFX_GRIN_RAD = 1 //!< Radians
} BFX_GrinUnit;

/**
 * @brief Data structure for Grin language.
 */
typedef struct {
    BFX_GrinUnit unit; //!< Unit of measurement.
    double       reg; //!< Register value.
    int          precision; //!< Number of decimal places to print.
    double       tape[BFX_DEFAULT_TAPE_SIZE]; //!< Tape memory.
} BFX_GrinData;

#ifndef BFX_GRIN_DEFAULT_PRECISION
/**
 * @brief Default number of decimal places to print.
 */
#define BFX_GRIN_DEFAULT_PRECISION 4
#endif

void bfx_grin_init(BFX*);
void bfx_grin_run(BFX*);

/* ops */
void bfx_op_grin_inc_t(BFX*, BFX_FileIndex*);
void bfx_op_grin_dec_t(BFX*, BFX_FileIndex*);
void bfx_op_grin_loop_start(BFX*, BFX_FileIndex*);
void bfx_op_grin_loop_end(BFX*, BFX_FileIndex*);
void bfx_op_grin_putchar_ascii(BFX*, BFX_FileIndex*);
void bfx_op_grin_putchar_number(BFX*, BFX_FileIndex*);
void bfx_op_grin_getchar_ascii(BFX*, BFX_FileIndex*);
void bfx_op_grin_getchar_number(BFX*, BFX_FileIndex*);
void bfx_op_grin_putchar_register_ascii(BFX*, BFX_FileIndex*);
void bfx_op_grin_putchar_register_number(BFX*, BFX_FileIndex*);
void bfx_op_grin_add(BFX*, BFX_FileIndex*);
void bfx_op_grin_sub(BFX*, BFX_FileIndex*);
void bfx_op_grin_mul(BFX*, BFX_FileIndex*);
void bfx_op_grin_div(BFX*, BFX_FileIndex*);
void bfx_op_grin_exp(BFX*, BFX_FileIndex*);
void bfx_op_grin_store(BFX*, BFX_FileIndex*);
void bfx_op_grin_load(BFX*, BFX_FileIndex*);
void bfx_op_grin_swap(BFX*, BFX_FileIndex*);
void bfx_op_grin_zero(BFX*, BFX_FileIndex*);
void bfx_op_grin_zero_ptr(BFX*, BFX_FileIndex*);
void bfx_op_grin_mod(BFX*, BFX_FileIndex*);
void bfx_op_grin_round(BFX*, BFX_FileIndex*);
void bfx_op_grin_nand(BFX*, BFX_FileIndex*);
void bfx_op_grin_or(BFX*, BFX_FileIndex*);
void bfx_op_grin_not(BFX*, BFX_FileIndex*);
void bfx_op_grin_negate(BFX*, BFX_FileIndex*);
void bfx_op_grin_simplify(BFX*, BFX_FileIndex*);
void bfx_op_grin_print(BFX*, BFX_FileIndex*);
void bfx_op_grin_newline(BFX*, BFX_FileIndex*);
void bfx_op_grin_sin(BFX*, BFX_FileIndex*);
void bfx_op_grin_cos(BFX*, BFX_FileIndex*);
void bfx_op_grin_tan(BFX*, BFX_FileIndex*);
void bfx_op_grin_arcsin(BFX*, BFX_FileIndex*);
void bfx_op_grin_arccos(BFX*, BFX_FileIndex*);
void bfx_op_grin_arctan(BFX*, BFX_FileIndex*);
void bfx_op_grin_inv(BFX*, BFX_FileIndex*);
void bfx_op_grin_mod2(BFX*, BFX_FileIndex*);
void bfx_op_grin_e(BFX*, BFX_FileIndex*);
void bfx_op_grin_pi(BFX*, BFX_FileIndex*);
void bfx_op_grin_ln(BFX*, BFX_FileIndex*);
void bfx_op_grin_mean(BFX*, BFX_FileIndex*);
void bfx_op_grin_sqrt(BFX*, BFX_FileIndex*);
void bfx_op_grin_log(BFX*, BFX_FileIndex*);
void bfx_op_grin_jump(BFX*, BFX_FileIndex*);
void bfx_op_grin_toggle_deg_rad(BFX*, BFX_FileIndex*);
void bfx_op_grin_exit(BFX*, BFX_FileIndex*);

#endif
