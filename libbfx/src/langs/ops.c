/**
 * @file langs/ops.c
 *
 * This file contains all ops for the supported languages.
 */

#include "ops.h"

#include "bfx.h"
#include "grin.h"
#include "pbrain.h"

#include <math.h>
#include <stdio.h>
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

void bfx_op_getchar_ascii(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = getchar();
}

void bfx_op_getchar_number(BFX* bf, BFX_FileIndex* index) {
    // TODO implement
}

void bfx_op_putchar_register_ascii(BFX* bf, BFX_FileIndex* index) {
    putchar((char) BFX_GRIN_DATA->reg);
}

void bfx_op_putchar_register_number(BFX* bf, BFX_FileIndex* index) {
    printf("%.*f", BFX_GRIN_DATA->precision, BFX_GRIN_DATA->reg);
}

void bfx_op_add(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] += BFX_GRIN_DATA->reg;
}

void bfx_op_sub(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] -= BFX_GRIN_DATA->reg;
}

void bfx_op_mul(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] *= BFX_GRIN_DATA->reg;
}

void bfx_op_div(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] /= BFX_GRIN_DATA->reg;
}

void bfx_op_exp(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = pow(BFX_GRIN_DATA->tape[bf->tp], BFX_GRIN_DATA->reg);
}

void bfx_op_store(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->reg = BFX_GRIN_DATA->tape[bf->tp];
}

void bfx_op_load(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_GRIN_DATA->reg;
}

void bfx_op_swap(BFX* bf, BFX_FileIndex* index) {
    double temp                 = BFX_GRIN_DATA->tape[bf->tp];
    BFX_GRIN_DATA->tape[bf->tp] = BFX_GRIN_DATA->reg;
    BFX_GRIN_DATA->reg          = temp;
}

void bfx_op_zero(BFX* bf, BFX_FileIndex* index) { BFX_GRIN_DATA->tape[bf->tp] = 0; }

void bfx_op_zero_ptr(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[(int) BFX_GRIN_DATA->reg] = 0;
}

void bfx_op_mod(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = fmod(BFX_GRIN_DATA->tape[bf->tp], BFX_GRIN_DATA->reg);
}

void bfx_op_round(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = round(BFX_GRIN_DATA->tape[bf->tp]);
}

void bfx_op_nand(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp]
        = ~(((int) BFX_GRIN_DATA->tape[bf->tp]) & ((int) BFX_GRIN_DATA->reg));
}

void bfx_op_or(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = ((int) BFX_GRIN_DATA->tape[bf->tp]) | ((int) BFX_GRIN_DATA->reg);
}

void bfx_op_not(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = ~((int) BFX_GRIN_DATA->tape[bf->tp]);
}

void bfx_op_negate(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = -((int) BFX_GRIN_DATA->tape[bf->tp]);
}

void bfx_op_simplify(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = (BFX_GRIN_DATA->tape[bf->tp] < -1.0)
                                      ? -1.0
                                      : ((BFX_GRIN_DATA->tape[bf->tp] > 1.0) ? 1.0 : 0.0);
}

void bfx_op_print(BFX* bf, BFX_FileIndex* index) {
    bf->ip++;
    while (((size_t) bf->ip) < bf->program_len && bf->program[bf->ip] != ')') {
        putchar(bf->program[bf->ip++]);
    }
    putchar('\n');
}

void bfx_op_newline(BFX* bf, BFX_FileIndex* index) { putchar('\n'); }

void bfx_op_sin(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_CONVERT(sin(BFX_GRIN_DATA->tape[bf->tp]));
}

void bfx_op_cos(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_CONVERT(cos(BFX_GRIN_DATA->tape[bf->tp]));
}

void bfx_op_tan(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_CONVERT(tan(BFX_GRIN_DATA->tape[bf->tp]));
}

void bfx_op_arcsin(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_CONVERT(asin(BFX_GRIN_DATA->tape[bf->tp]));
}

void bfx_op_arccos(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_CONVERT(acos(BFX_GRIN_DATA->tape[bf->tp]));
}

void bfx_op_arctan(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = atan(BFX_GRIN_DATA->tape[bf->tp]);
}

void bfx_op_inv(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = 1 / BFX_GRIN_DATA->tape[bf->tp];
}

void bfx_op_mod2(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = BFX_CONVERT(fmod(BFX_GRIN_DATA->tape[bf->tp], 2));
}

void bfx_op_e(BFX* bf, BFX_FileIndex* index) { BFX_GRIN_DATA->tape[bf->tp] = M_E; }

void bfx_op_pi(BFX* bf, BFX_FileIndex* index) { BFX_GRIN_DATA->tape[bf->tp] = M_PI; }

void bfx_op_ln(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = log(BFX_GRIN_DATA->tape[bf->tp]);
}

void bfx_op_mean(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = (BFX_GRIN_DATA->tape[bf->tp] + BFX_GRIN_DATA->reg) / 2;
}

void bfx_op_sqrt(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = sqrt(BFX_GRIN_DATA->tape[bf->tp]);
}

void bfx_op_log(BFX* bf, BFX_FileIndex* index) {
    BFX_GRIN_DATA->tape[bf->tp] = log10(BFX_GRIN_DATA->tape[bf->tp]);
}

void bfx_op_jump(BFX* bf, BFX_FileIndex* index) {
    if (BFX_GRIN_DATA->reg > 0) {
        bf->ip += BFX_GRIN_DATA->reg;
    }
}

void bfx_op_toggle_deg_rad(BFX* bf, BFX_FileIndex* index) {
    if (BFX_GRIN_DATA->unit == BFX_GRIN_DEG) {
        BFX_GRIN_DATA->unit = BFX_GRIN_RAD;
    } else {
        BFX_GRIN_DATA->unit = BFX_GRIN_DEG;
    }
}

void bfx_op_exit(BFX* bf, BFX_FileIndex* index) { bf->ip = bf->program_len; }
