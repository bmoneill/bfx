/**
 * @file langs/ops.c
 *
 * This file contains all ops for the supported languages.
 */

#include "ops.h"

#include "bfx.h"
#include "grin.h"
#include "pbrain.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define BFX_T                                                                                      \
    (bf->lang == BFX_LANG_GRIN ? ((int) ((BFX_GrinData*) bf->lang_data)->tape[bf->tp])             \
                               : bf->tape[bf->tp])

#define BFX_GRIN_DATA ((BFX_GrinData*) bf->lang_data)

#define BFX_CONVERT(a) ((BFX_GRIN_DATA->unit == BFX_GRIN_DEG) ? (a * M_PI / 180.0) : a)

/**
 * @brief Increment tape pointer (brainfuck).
 */
void bfx_op_inc_tp(BFX* bf, BFX_FileIndex* index) {
    bf->tp++;
    if ((size_t) bf->tp > bf->tape_size) {
        fprintf(stderr,
                "Warning (%d,%d): Tape pointer overflow. Tape pointer set to zero.\n",
                index->line,
                index->line_idx);
        bf->tp = 0;
    } else if (bf->tp > bf->tp_max) {
        bf->tp_max = bf->tp;
    }
}

/**
 * @brief Decrement tape pointer (brainfuck).
 */
void bfx_op_dec_tp(BFX* bf, BFX_FileIndex* index) {
    bf->tp--;
    if (bf->tp < 0) {
        fprintf(stderr,
                "Warning (%d,%d): Tape pointer underflow. Tape pointer set to zero.\n",
                index->line,
                index->line_idx);
        bf->tp = 0;
    }
}

/**
 * @brief Increment tape value (brainfuck).
 */
void bfx_op_inc_t(BFX* bf, BFX_FileIndex* index) { bf->tape[BFX_T]++; }

/**
 * @brief Decrement tape value (brainfuck).
 */
void bfx_op_dec_t(BFX* bf, BFX_FileIndex* index) { bf->tape[BFX_T]--; }

/**
 * @brief Start of loop (brainfuck).
 */
void bfx_op_loop_start(BFX* bf, BFX_FileIndex* index) {
    if (!bf->tape[BFX_T]) {
        for (size_t i = 0; i < bf->loops_len; i++) {
            if (bf->loops[i].start.idx == bf->ip) {
                bf->ip          = bf->loops[i].end.idx;
                index->line     = bf->loops[i].end.line;
                index->line_idx = bf->loops[i].end.line_idx;
            }
        }
    }
}

/**
 * @brief End of loop (brainfuck).
 */
void bfx_op_loop_end(BFX* bf, BFX_FileIndex* index) {
    if (bf->tape[BFX_T]) {
        for (size_t i = 0; i < bf->loops_len; i++) {
            if (bf->loops[i].end.idx == bf->ip) {
                bf->ip          = bf->loops[i].start.idx;
                index->line     = bf->loops[i].start.line;
                index->line_idx = bf->loops[i].start.line_idx;
            }
        }
    }
}

/**
 * @brief Get character from input or stdin (brainfuck).
 */
void bfx_op_getchar(BFX* bf, BFX_FileIndex* index) {
    char c;
    if (bf->flags & BFX_FLAG_SEPARATE_INPUT_AND_SOURCE) {
        if (bf->input_ptr < bf->input_len) {
            c = bf->program[bf->input_ptr];
            bf->input_ptr++;
        } else {
            c = EOF;
        }
    } else {
        c = fgetc(stdin);
    }

    if (bf->receiving) {
        if (c == EOF) {
            bf->receiving = false;
        } else {
            bf->tape[bf->tp] = c;
        }
    }

    if (!bf->receiving) {
        switch (bf->eof_behavior) {
        case BFX_EOF_BEHAVIOR_ZERO:
            bf->tape[bf->tp] = 0;
            break;
        case BFX_EOF_BEHAVIOR_DECREMENT:
            bf->tape[bf->tp]--;
            break;
        }
    }
}

/**
 * @brief Output the current cell value as a character (brainfuck).
 */
void bfx_op_putchar(BFX* bf, BFX_FileIndex* index) { putchar(bf->tape[bf->tp]); }

/**
 * @brief Begin procedure definition (pbrain).
 *
 * @param bf Pointer to the interpreter struct
 * @param idx Pointer to the file index struct
 */
void bfx_op_start_procedure(BFX* bf, BFX_FileIndex* idx) {
    size_t          i;
    BFX_PBrainData* data                              = (BFX_PBrainData*) bf->lang_data;

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
 * @brief Call a procedure by its identifier (pbrain).
 */
void bfx_op_call(BFX* bf, BFX_FileIndex* index) {
    BFX_PBrainData* data = (BFX_PBrainData*) bf->lang_data;
    for (size_t i = 0; i < data->procedures_len; i++) {
        if (data->procedures[i].identifier == bf->tape[bf->tp]) {
            data->stack_top++;
            data->stack[data->stack_top] = bf->ip;
            bf->ip                       = data->procedures[i].start_idx;
            return;
        }
    }
}

/**
 * @brief Return from a procedure (pbrain).
 */
void bfx_op_ret(BFX* bf, BFX_FileIndex* index) {
    BFX_PBrainData* data = (BFX_PBrainData*) bf->lang_data;
    if (data->stack_top > 0) {
        bf->ip = data->stack[data->stack_top];
        data->stack_top--;
    }
}

/**
 * @brief Toggle between thread and global tape (weave).
 */
void bfx_op_toggle(BFX* bf, BFX_FileIndex* index) {
    uint8_t* old_tape = bf->tape;
    bf->tape          = bf->lang_data;
    bf->lang_data     = old_tape;
}

/**
 * @brief Output current cell as ASCII character (Grin)
 */
void bfx_op_putchar_ascii(BFX* bf, BFX_FileIndex* index) {
    putchar((char) BFX_GRIN_DATA->tape[bf->tp]);
}

/**
 * @brief Output current cell as a number (Grin)
 */
void bfx_op_putchar_number(BFX* bf, BFX_FileIndex* index) {
    printf("%.*f", BFX_GRIN_DATA->precision, BFX_GRIN_DATA->tape[bf->tp]);
}

/**
 * @brief Set current cell to inputted ASCII character (Grin)
 */
void bfx_op_getchar_ascii(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = getchar();
}

/**
 * @brief Set current cell to inputted number (Grin)
 */
void bfx_op_getchar_number(BFX* bf, BFX_FileIndex* index) {
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
        s[i + 1]                    = '\0';
        BFX_GRIN_DATA->tape[bf->tp] = atof(s);
    }
}

/**
 * @brief Print register as ASCII character (Grin)
 */
void bfx_op_putchar_register_ascii(BFX* bf, BFX_FileIndex* index) {
    putchar((char) BFX_GRIN_DATA->reg);
}

/**
 * @brief Print register as number (Grin)
 */
void bfx_op_putchar_register_number(BFX* bf, BFX_FileIndex* index) {
    printf("%.*f", BFX_GRIN_DATA->precision, BFX_GRIN_DATA->reg);
}

/**
 * @brief Add register to current cell (Grin)
 */
void bfx_op_add(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] += BFX_GRIN_DATA->reg;
}

/**
 * @brief Subtract register from current cell (Grin)
 */
void bfx_op_sub(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] -= BFX_GRIN_DATA->reg;
}

/**
 * @brief Multiply current cell by register (Grin)
 */
void bfx_op_mul(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] *= BFX_GRIN_DATA->reg;
}

/**
 * @brief Divide current cell by register (Grin)
 */
void bfx_op_div(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] /= BFX_GRIN_DATA->reg;
}

/**
 * @brief Raise current cell to the power of register (Grin)
 */
void bfx_op_exp(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = pow(BFX_GRIN_DATA->tape[bf->tp], BFX_GRIN_DATA->reg);
}

/**
 * @brief Store current cell value in register (Grin)
 */
void bfx_op_store(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->reg = BFX_GRIN_DATA->tape[bf->tp];
}

/**
 * @brief Store register value in current cell (Grin)
 */
void bfx_op_load(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_GRIN_DATA->reg;
}

/**
 * @brief Swap current cell value with register value (Grin)
 */
void bfx_op_swap(BFX* bf, BFX_FileIndex* index) {
    double temp                 = BFX_GRIN_DATA->tape[bf->tp];
    BFX_GRIN_DATA->tape[bf->tp] = BFX_GRIN_DATA->reg;
    BFX_GRIN_DATA->reg          = temp;
}

/**
 * @brief Zero current cell value (Grin)
 */
void bfx_op_zero(BFX* bf, BFX_FileIndex* index) { BFX_GRIN_DATA->tape[bf->tp] = 0; }

/**
 * @brief Zero cell at register value (Grin)
 */
void bfx_op_zero_ptr(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[(int) BFX_GRIN_DATA->reg] = 0;
}

/**
 * @brief Modulo (Grin)
 */
void bfx_op_mod(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = fmod(BFX_GRIN_DATA->tape[bf->tp], BFX_GRIN_DATA->reg);
}

/**
 * @brief Round current cell value (Grin)
 */
void bfx_op_round(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = round(BFX_GRIN_DATA->tape[bf->tp]);
}

/**
 * @brief NAND current cell value with register value (Grin)
 */
void bfx_op_nand(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp]
        = ~(((int) BFX_GRIN_DATA->tape[bf->tp]) & ((int) BFX_GRIN_DATA->reg));
}

/**
 * @brief OR current cell value with register value (Grin)
 */
void bfx_op_or(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = ((int) BFX_GRIN_DATA->tape[bf->tp]) | ((int) BFX_GRIN_DATA->reg);
}

/**
 * @brief NOT current cell value (Grin)
 */
void bfx_op_not(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = ~((int) BFX_GRIN_DATA->tape[bf->tp]);
}

/**
 * @brief Negate current cell value (Grin)
 */
void bfx_op_negate(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = -BFX_GRIN_DATA->tape[bf->tp];
}

/**
 * @brief Simplify current cell value (set to -1.0 if < -1.0, 1.0 if > 1.0, 0.0 otherwise) (Grin)
 */
void bfx_op_simplify(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = (BFX_GRIN_DATA->tape[bf->tp] <= -1.0)
                                      ? -1.0
                                      : ((BFX_GRIN_DATA->tape[bf->tp] >= 1.0) ? 1.0 : 0.0);
}

/**
 * @brief Print from instruction pointer to next ')' (Grin)
 */
void bfx_op_print(BFX* bf, BFX_FileIndex* index) {
    bf->ip++;
    while (((size_t) bf->ip) < bf->program_len && bf->program[bf->ip] != ')') {
        putchar(bf->program[bf->ip++]);
    }
    putchar('\n');
}

/**
 * @brief Print a newline (Grin)
 */
void bfx_op_newline(BFX* bf, BFX_FileIndex* index) { putchar('\n'); }

/**
 * @brief Sine of current cell value (Grin)
 */
void bfx_op_sin(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_CONVERT(sin(BFX_GRIN_DATA->tape[bf->tp]));
}

/**
 * @brief Cosine of current cell value (Grin)
 */
void bfx_op_cos(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_CONVERT(cos(BFX_GRIN_DATA->tape[bf->tp]));
}

/**
 * @brief Tangent of current cell value (Grin)
 */
void bfx_op_tan(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_CONVERT(tan(BFX_GRIN_DATA->tape[bf->tp]));
}

/**
 * @brief Arcsine of current cell value (Grin)
 */
void bfx_op_arcsin(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_CONVERT(asin(BFX_GRIN_DATA->tape[bf->tp]));
}

/**
 * @brief Arccosine of current cell value (Grin)
 */
void bfx_op_arccos(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_CONVERT(acos(BFX_GRIN_DATA->tape[bf->tp]));
}

/**
 * @brief Arctangent of current cell value (Grin)
 */
void bfx_op_arctan(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = atan(BFX_GRIN_DATA->tape[bf->tp]);
}

/**
 * @brief Inverse of current cell value (Grin)
 */
void bfx_op_inv(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = 1 / BFX_GRIN_DATA->tape[bf->tp];
}

/**
 * @brief Modulo of current cell value by 2 (Grin)
 */
void bfx_op_mod2(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_CONVERT(fmod(BFX_GRIN_DATA->tape[bf->tp], 2));
}

/**
 * @brief Set current cell to Euler's number (Grin)
 */
void bfx_op_e(BFX* bf, BFX_FileIndex* index) { BFX_GRIN_DATA->tape[bf->tp] = M_E; }

/**
 * @brief Set current cell to Pi (Grin)
 */
void bfx_op_pi(BFX* bf, BFX_FileIndex* index) { BFX_GRIN_DATA->tape[bf->tp] = M_PI; }

/**
 * @brief Natural logarithm of current cell value (Grin)
 */
void bfx_op_ln(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = log(BFX_GRIN_DATA->tape[bf->tp]);
}

/**
 * @brief Mean of current cell value and register value (Grin)
 */
void bfx_op_mean(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = (BFX_GRIN_DATA->tape[bf->tp] + BFX_GRIN_DATA->reg) / 2;
}

/**
 * @brief Square root of current cell value (Grin)
 */
void bfx_op_sqrt(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = sqrt(BFX_GRIN_DATA->tape[bf->tp]);
}

/**
 * @brief Base-10 logarithm of current cell value (Grin)
 */
void bfx_op_log(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = log10(BFX_GRIN_DATA->tape[bf->tp]);
}

/**
 * @brief Jump n instructions, n=register value (Grin)
 */
void bfx_op_jump(BFX* bf, BFX_FileIndex* index) {
    if (BFX_GRIN_DATA->reg > 0) {
        bf->ip += BFX_GRIN_DATA->reg;
    }
}

/**
 * @brief Toggle between degrees and radians (Grin)
 */
void bfx_op_toggle_deg_rad(BFX* bf, BFX_FileIndex* index) {
    if (BFX_GRIN_DATA->unit == BFX_GRIN_DEG) {
        BFX_GRIN_DATA->unit = BFX_GRIN_RAD;
    } else {
        BFX_GRIN_DATA->unit = BFX_GRIN_DEG;
    }
}

/**
 * @brief Exit the program (Grin)
 */
void bfx_op_exit(BFX* bf, BFX_FileIndex* index) { bf->ip = bf->program_len; }
