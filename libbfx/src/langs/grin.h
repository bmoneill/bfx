/**
 * @file langs/grin.h
 *
 * This file contains function definitions related to interpreting the Grin language.
 */
#ifndef BFX_LANGS_GRIN_H
#define BFX_LANGS_GRIN_H

#include "../bfx.h"

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

#define BFX_T                                                                                      \
    (bf->lang == BFX_LANG_GRIN ? ((int) ((BFX_GrinData*) bf->lang_data)->tape[bf->tp])             \
                               : bf->tape[bf->tp])

void bfx_grin_init(BFX*);
void bfx_grin_run(BFX*);

/* ops */
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
