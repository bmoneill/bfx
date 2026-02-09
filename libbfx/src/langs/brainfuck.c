/**
 * @file langs/bf.c
 *
 * This file contains the implementation of the Brainfuck interpreter.
 */

#include "../bfx.h"
#include "brainfuck.h"
#include "grin.h"
#include "util.h"

#include <stdio.h>

/**
 * @brief Initialize the Brainfuck interpreter
 * @param bfx Pointer to the already-allocated interpreter struct
 */
void bfx_brainfuck_init(BFX* bfx) { bfx_build_loops(bfx); }

/**
 * @brief Run the Brainfuck interpreter
 * @param bfx Pointer to the interpreter struct
 */
void bfx_brainfuck_run(BFX* bfx) {
    void (*ops[128])(BFX*, BFX_FileIndex*) = {
        [']'] = bfx_op_loop_end, ['['] = bfx_op_loop_start, ['+'] = bfx_op_inc_t,
        ['-'] = bfx_op_dec_t,    ['>'] = bfx_op_inc_tp,     ['<'] = bfx_op_dec_tp,
        [','] = bfx_op_getchar,  ['.'] = bfx_op_putchar,
    };
    bfx_parse_ops(bfx, ops);
}

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
