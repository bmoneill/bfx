/**
 * @file langs/grin.c
 *
 * This file contains the implementation of the Grin interpreter.
 */

#include "grin.h"

#include "bfx.h"
#include "brainfuck.h"
#include "util.h"

#include <ctype.h>
#include <math.h>

#define BFX_GRIN_DATA  ((BFX_GrinData*) bfx->lang_data)
#define BFX_CONVERT(a) ((BFX_GRIN_DATA->unit == BFX_GRIN_DEG) ? (a * M_PI / 180.0) : a)

/**
 * @brief Initialize the Grin interpreter
 *
 * @param bfx Pointer to the already-allocated interpreter struct
 */
BFX_Error bfx_grin_init(BFX* bfx) {
    if (bfx->tape) {
        free(bfx->tape);
    }

    bfx->lang_data = calloc(1, sizeof(BFX_GrinData));
    if (!bfx->lang_data) {
        fprintf(stderr, "Failed to allocate memory for Grin data\n");
        return BFX_RUNTIME_ERROR;
    }
    BFX_GrinData* data = (BFX_GrinData*) bfx->lang_data;
    data->unit         = bfx->flags & BFX_FLAG_DEGREES ? BFX_GRIN_DEG : BFX_GRIN_RAD;
    data->precision    = BFX_GRIN_DEFAULT_PRECISION;
    bfx_build_loops(bfx);
    return BFX_SUCCESS;
}

/**
 * @brief Run the Grin interpreter
 *
 * This function initializes and runs the Grin interpreter.
 *
 * @param bfx Pointer to the interpreter struct
 * @return Error code
 */
BFX_Error bfx_grin_run(BFX* bfx) {
    BFX_Error (*ops[128])(BFX*, BFX_FileIndex*) = { ['<']  = bfx_op_brainfuck_dec_tp,
                                                    ['>']  = bfx_op_brainfuck_inc_tp,
                                                    ['.']  = bfx_op_grin_putchar_ascii,
                                                    [':']  = bfx_op_grin_putchar_number,
                                                    [',']  = bfx_op_grin_getchar_ascii,
                                                    [';']  = bfx_op_grin_getchar_number,
                                                    ['\''] = bfx_op_grin_putchar_register_ascii,
                                                    ['"']  = bfx_op_grin_putchar_register_number,
                                                    ['+']  = bfx_op_grin_add,
                                                    ['-']  = bfx_op_grin_sub,
                                                    ['*']  = bfx_op_grin_mul,
                                                    ['/']  = bfx_op_grin_div,
                                                    ['^']  = bfx_op_grin_exp,
                                                    ['$']  = bfx_op_grin_store,
                                                    ['\\'] = bfx_op_grin_load,
                                                    ['~']  = bfx_op_grin_swap,
                                                    ['=']  = bfx_op_grin_zero,
                                                    ['_']  = bfx_op_grin_zero_ptr,
                                                    ['%']  = bfx_op_grin_mod,
                                                    ['r']  = bfx_op_grin_round,
                                                    ['&']  = bfx_op_grin_nand,
                                                    ['|']  = bfx_op_grin_or,
                                                    ['!']  = bfx_op_grin_not,
                                                    ['@']  = bfx_op_grin_negate,
                                                    ['[']  = bfx_op_grin_loop_start,
                                                    [']']  = bfx_op_grin_loop_end,
                                                    ['?']  = bfx_op_grin_simplify,
                                                    ['}']  = bfx_op_grin_inc_t,
                                                    ['{']  = bfx_op_grin_dec_t,
                                                    ['(']  = bfx_op_grin_print,
                                                    [')']  = bfx_op_grin_newline,
                                                    ['s']  = bfx_op_grin_sin,
                                                    ['c']  = bfx_op_grin_cos,
                                                    ['t']  = bfx_op_grin_tan,
                                                    ['S']  = bfx_op_grin_arcsin,
                                                    ['C']  = bfx_op_grin_arccos,
                                                    ['T']  = bfx_op_grin_arctan,
                                                    ['1']  = bfx_op_grin_inv,
                                                    ['2']  = bfx_op_grin_mod2,
                                                    ['e']  = bfx_op_grin_e,
                                                    ['p']  = bfx_op_grin_pi,
                                                    ['l']  = bfx_op_grin_ln,
                                                    ['m']  = bfx_op_grin_mean,
                                                    ['q']  = bfx_op_grin_sqrt,
                                                    ['L']  = bfx_op_grin_log,
                                                    ['j']  = bfx_op_grin_jump,
                                                    ['D']  = bfx_op_grin_toggle_deg_rad,
                                                    ['`']  = bfx_op_grin_exit };

    return bfx_parse_ops(bfx, ops);
}

/**
 * @brief Increment value at tape pointer (Grin).
 */
BFX_Error bfx_op_grin_inc_t(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp]++;
    return BFX_SUCCESS;
}

/**
 * @brief Decrement value at tape pointer (Grin).
 */
BFX_Error bfx_op_grin_dec_t(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp]--;
    return BFX_SUCCESS;
}

/**
 * @brief Start of loop (Grin).
 */
BFX_Error bfx_op_grin_loop_start(BFX* bfx, BFX_FileIndex* index) {
    if (!BFX_GRIN_DATA->tape[bfx->tp]) {
        for (size_t i = 0; i < bfx->loops_len; i++) {
            if (bfx->loops[i].start.idx == bfx->ip) {
                bfx->ip         = bfx->loops[i].end.idx;
                index->line     = bfx->loops[i].end.line;
                index->line_idx = bfx->loops[i].end.line_idx;
                return BFX_SUCCESS;
            }
        }
        fprintf(stderr,
                "Unmatched '[' encountered at line (%d,%d)\n",
                index->line,
                index->line_idx);
        return BFX_SYNTAX_ERROR;
    }
    return BFX_SUCCESS;
}

/**
 * @brief End of loop (Grin).
 */
BFX_Error bfx_op_grin_loop_end(BFX* bfx, BFX_FileIndex* index) {
    if (BFX_GRIN_DATA->tape[bfx->tp]) {
        for (size_t i = 0; i < bfx->loops_len; i++) {
            if (bfx->loops[i].end.idx == bfx->ip) {
                bfx->ip         = bfx->loops[i].start.idx;
                index->line     = bfx->loops[i].start.line;
                index->line_idx = bfx->loops[i].start.line_idx;
                return BFX_SUCCESS;
            }
        }
        fprintf(stderr,
                "Unmatched ']' encountered at line (%d,%d)\n",
                index->line,
                index->line_idx);
        return BFX_SYNTAX_ERROR;
    }
    return BFX_SUCCESS;
}

/**
 * @brief Output current cell as ASCII character (Grin)
 */
BFX_Error bfx_op_grin_putchar_ascii(BFX* bfx, BFX_FileIndex* index) {
    putchar((char) BFX_GRIN_DATA->tape[bfx->tp]);
    return BFX_SUCCESS;
}

/**
 * @brief Output current cell as a number (Grin)
 */
BFX_Error bfx_op_grin_putchar_number(BFX* bfx, BFX_FileIndex* index) {
    printf("%.*f", BFX_GRIN_DATA->precision, BFX_GRIN_DATA->tape[bfx->tp]);
    return BFX_SUCCESS;
}

/**
 * @brief Set current cell to inputted ASCII character (Grin)
 */
BFX_Error bfx_op_grin_getchar_ascii(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = getchar();
    return BFX_SUCCESS;
}

/**
 * @brief Set current cell to inputted number (Grin)
 */
BFX_Error bfx_op_grin_getchar_number(BFX* bfx, BFX_FileIndex* index) {
    char s[64];
    int  c, i = 0;
    while ((c = fgetc(stdin)) != EOF) {
        if (isdigit(c) && c == '.') {
            s[i++] = c;
        } else {
            break;
        }
    }

    if (i > 0) {
        s[i + 1]                     = '\0';
        BFX_GRIN_DATA->tape[bfx->tp] = atof(s);
    }
    return BFX_SUCCESS;
}

/**
 * @brief Print register as ASCII character (Grin)
 */
BFX_Error bfx_op_grin_putchar_register_ascii(BFX* bfx, BFX_FileIndex* index) {
    putchar((char) BFX_GRIN_DATA->reg);
    return BFX_SUCCESS;
}

/**
 * @brief Print register as number (Grin)
 */
BFX_Error bfx_op_grin_putchar_register_number(BFX* bfx, BFX_FileIndex* index) {
    printf("%.*f", BFX_GRIN_DATA->precision, BFX_GRIN_DATA->reg);
    return BFX_SUCCESS;
}

/**
 * @brief Add register to current cell (Grin)
 */
BFX_Error bfx_op_grin_add(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] += BFX_GRIN_DATA->reg;
    return BFX_SUCCESS;
}

/**
 * @brief Subtract register from current cell (Grin)
 */
BFX_Error bfx_op_grin_sub(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] -= BFX_GRIN_DATA->reg;
    return BFX_SUCCESS;
}

/**
 * @brief Multiply current cell by register (Grin)
 */
BFX_Error bfx_op_grin_mul(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] *= BFX_GRIN_DATA->reg;
    return BFX_SUCCESS;
}

/**
 * @brief Divide current cell by register (Grin)
 */
BFX_Error bfx_op_grin_div(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] /= BFX_GRIN_DATA->reg;
    return BFX_SUCCESS;
}

/**
 * @brief Raise current cell to the power of register (Grin)
 */
BFX_Error bfx_op_grin_exp(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = pow(BFX_GRIN_DATA->tape[bfx->tp], BFX_GRIN_DATA->reg);
    return BFX_SUCCESS;
}

/**
 * @brief Store current cell value in register (Grin)
 */
BFX_Error bfx_op_grin_store(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->reg = BFX_GRIN_DATA->tape[bfx->tp];
    return BFX_SUCCESS;
}

/**
 * @brief Store register value in current cell (Grin)
 */
BFX_Error bfx_op_grin_load(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = BFX_GRIN_DATA->reg;
    return BFX_SUCCESS;
}

/**
 * @brief Swap current cell value with register value (Grin)
 */
BFX_Error bfx_op_grin_swap(BFX* bfx, BFX_FileIndex* index) {
    double temp                  = BFX_GRIN_DATA->tape[bfx->tp];
    BFX_GRIN_DATA->tape[bfx->tp] = BFX_GRIN_DATA->reg;
    BFX_GRIN_DATA->reg           = temp;
    return BFX_SUCCESS;
}

/**
 * @brief Zero current cell value (Grin)
 */
BFX_Error bfx_op_grin_zero(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = 0;
    return BFX_SUCCESS;
}

/**
 * @brief Zero cell at register value (Grin)
 */
BFX_Error bfx_op_grin_zero_ptr(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[(int) BFX_GRIN_DATA->reg] = 0;
    return BFX_SUCCESS;
}

/**
 * @brief Modulo (Grin)
 */
BFX_Error bfx_op_grin_mod(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = fmod(BFX_GRIN_DATA->tape[bfx->tp], BFX_GRIN_DATA->reg);
    return BFX_SUCCESS;
}

/**
 * @brief Round current cell value (Grin)
 */
BFX_Error bfx_op_grin_round(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = round(BFX_GRIN_DATA->tape[bfx->tp]);
    return BFX_SUCCESS;
}

/**
 * @brief NAND current cell value with register value (Grin)
 */
BFX_Error bfx_op_grin_nand(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp]
        = ~(((int) BFX_GRIN_DATA->tape[bfx->tp]) & ((int) BFX_GRIN_DATA->reg));
    return BFX_SUCCESS;
}

/**
 * @brief OR current cell value with register value (Grin)
 */
BFX_Error bfx_op_grin_or(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp]
        = ((int) BFX_GRIN_DATA->tape[bfx->tp]) | ((int) BFX_GRIN_DATA->reg);
    return BFX_SUCCESS;
}

/**
 * @brief NOT current cell value (Grin)
 */
BFX_Error bfx_op_grin_not(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = ~((int) BFX_GRIN_DATA->tape[bfx->tp]);
    return BFX_SUCCESS;
}

/**
 * @brief Negate current cell value (Grin)
 */
BFX_Error bfx_op_grin_negate(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = -BFX_GRIN_DATA->tape[bfx->tp];
    return BFX_SUCCESS;
}

/**
 * @brief Simplify current cell value (set to -1.0 if < -1.0, 1.0 if > 1.0, 0.0 otherwise) (Grin)
 */
BFX_Error bfx_op_grin_simplify(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = (BFX_GRIN_DATA->tape[bfx->tp] <= -1.0)
                                       ? -1.0
                                       : ((BFX_GRIN_DATA->tape[bfx->tp] >= 1.0) ? 1.0 : 0.0);
    return BFX_SUCCESS;
}

/**
 * @brief Print from instruction pointer to next ')' (Grin)
 */
BFX_Error bfx_op_grin_print(BFX* bfx, BFX_FileIndex* index) {
    bfx->ip++;
    while (((size_t) bfx->ip) < bfx->program_len && bfx->program[bfx->ip] != ')') {
        putchar(bfx->program[bfx->ip++]);
    }
    putchar('\n');
    return BFX_SUCCESS;
}

/**
 * @brief Print a newline (Grin)
 */
BFX_Error bfx_op_grin_newline(BFX* bfx, BFX_FileIndex* index) {
    putchar('\n');
    return BFX_SUCCESS;
}

/**
 * @brief Sine of current cell value (Grin)
 */
BFX_Error bfx_op_grin_sin(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = BFX_CONVERT(sin(BFX_GRIN_DATA->tape[bfx->tp]));
    return BFX_SUCCESS;
}

/**
 * @brief Cosine of current cell value (Grin)
 */
BFX_Error bfx_op_grin_cos(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = BFX_CONVERT(cos(BFX_GRIN_DATA->tape[bfx->tp]));
    return BFX_SUCCESS;
}

/**
 * @brief Tangent of current cell value (Grin)
 */
BFX_Error bfx_op_grin_tan(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = BFX_CONVERT(tan(BFX_GRIN_DATA->tape[bfx->tp]));
    return BFX_SUCCESS;
}

/**
 * @brief Arcsine of current cell value (Grin)
 */
BFX_Error bfx_op_grin_arcsin(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = BFX_CONVERT(asin(BFX_GRIN_DATA->tape[bfx->tp]));
    return BFX_SUCCESS;
}

/**
 * @brief Arccosine of current cell value (Grin)
 */
BFX_Error bfx_op_grin_arccos(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = BFX_CONVERT(acos(BFX_GRIN_DATA->tape[bfx->tp]));
    return BFX_SUCCESS;
}

/**
 * @brief Arctangent of current cell value (Grin)
 */
BFX_Error bfx_op_grin_arctan(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = atan(BFX_GRIN_DATA->tape[bfx->tp]);
    return BFX_SUCCESS;
}

/**
 * @brief Inverse of current cell value (Grin)
 */
BFX_Error bfx_op_grin_inv(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = 1 / BFX_GRIN_DATA->tape[bfx->tp];
    return BFX_SUCCESS;
}

/**
 * @brief Modulo of current cell value by 2 (Grin)
 */
BFX_Error bfx_op_grin_mod2(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = BFX_CONVERT(fmod(BFX_GRIN_DATA->tape[bfx->tp], 2));
    return BFX_SUCCESS;
}

/**
 * @brief Set current cell to Euler's number (Grin)
 */
BFX_Error bfx_op_grin_e(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = M_E;
    return BFX_SUCCESS;
}

/**
 * @brief Set current cell to Pi (Grin)
 */
BFX_Error bfx_op_grin_pi(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = M_PI;
    return BFX_SUCCESS;
}

/**
 * @brief Natural logarithm of current cell value (Grin)
 */
BFX_Error bfx_op_grin_ln(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = log(BFX_GRIN_DATA->tape[bfx->tp]);
    return BFX_SUCCESS;
}

/**
 * @brief Mean of current cell value and register value (Grin)
 */
BFX_Error bfx_op_grin_mean(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = (BFX_GRIN_DATA->tape[bfx->tp] + BFX_GRIN_DATA->reg) / 2;
    return BFX_SUCCESS;
}

/**
 * @brief Square root of current cell value (Grin)
 */
BFX_Error bfx_op_grin_sqrt(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = sqrt(BFX_GRIN_DATA->tape[bfx->tp]);
    return BFX_SUCCESS;
}

/**
 * @brief Base-10 logarithm of current cell value (Grin)
 */
BFX_Error bfx_op_grin_log(BFX* bfx, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bfx->tp] = log10(BFX_GRIN_DATA->tape[bfx->tp]);
    return BFX_SUCCESS;
}

/**
 * @brief Jump n instructions, n=register value (Grin)
 */
BFX_Error bfx_op_grin_jump(BFX* bfx, BFX_FileIndex* index) {
    if (BFX_GRIN_DATA->reg > 0) {
        bfx->ip += BFX_GRIN_DATA->reg;
    }
    return BFX_SUCCESS;
}

/**
 * @brief Toggle between degrees and radians (Grin)
 */
BFX_Error bfx_op_grin_toggle_deg_rad(BFX* bfx, BFX_FileIndex* index) {
    if (BFX_GRIN_DATA->unit == BFX_GRIN_DEG) {
        BFX_GRIN_DATA->unit = BFX_GRIN_RAD;
    } else {
        BFX_GRIN_DATA->unit = BFX_GRIN_DEG;
    }
    return BFX_SUCCESS;
}

/**
 * @brief Exit the program (Grin)
 */
BFX_Error bfx_op_grin_exit(BFX* bfx, BFX_FileIndex* index) {
    bfx->ip = bfx->program_len;
    return BFX_SUCCESS;
}
