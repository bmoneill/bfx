/**
 * @file langs/bf.c
 *
 * This file contains the implementation of the Brainfuck interpreter.
 */

#include "brainfuck.h"
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
        [']'] = bfx_op_brainfuck_loop_end, ['['] = bfx_op_brainfuck_loop_start,
        ['+'] = bfx_op_brainfuck_inc_t,    ['-'] = bfx_op_brainfuck_dec_t,
        ['>'] = bfx_op_brainfuck_inc_tp,   ['<'] = bfx_op_brainfuck_dec_tp,
        [','] = bfx_op_brainfuck_getchar,  ['.'] = bfx_op_brainfuck_putchar,
    };
    bfx_parse_ops(bfx, ops);
}

/**
 * @brief Increment tape pointer (brainfuck).
 */
void bfx_op_brainfuck_inc_tp(BFX* bfx, BFX_FileIndex* index) {
    bfx->tp++;
    if ((size_t) bfx->tp > bfx->tape_size) {
        fprintf(stderr,
                "Warning (%d,%d): Tape pointer overflow. Tape pointer set to zero.\n",
                index->line,
                index->line_idx);
        bfx->tp = 0;
    } else if (bfx->tp > bfx->tp_max) {
        bfx->tp_max = bfx->tp;
    }
}

/**
 * @brief Decrement tape pointer (brainfuck).
 */
void bfx_op_brainfuck_dec_tp(BFX* bfx, BFX_FileIndex* index) {
    bfx->tp--;
    if (bfx->tp < 0) {
        fprintf(stderr,
                "Warning (%d,%d): Tape pointer underflow. Tape pointer set to zero.\n",
                index->line,
                index->line_idx);
        bfx->tp = 0;
    }
}

/**
 * @brief Increment tape value (brainfuck).
 */
void bfx_op_brainfuck_inc_t(BFX* bfx, BFX_FileIndex* index) { bfx->tape[bfx->tp]++; }

/**
 * @brief Decrement tape value (brainfuck).
 */
void bfx_op_brainfuck_dec_t(BFX* bfx, BFX_FileIndex* index) { bfx->tape[bfx->tp]--; }

/**
 * @brief Start of loop (brainfuck).
 */
void bfx_op_brainfuck_loop_start(BFX* bfx, BFX_FileIndex* index) {
    if (!bfx->tape[bfx->tp]) {
        for (size_t i = 0; i < bfx->loops_len; i++) {
            if (bfx->loops[i].start.idx == bfx->ip) {
                bfx->ip         = bfx->loops[i].end.idx;
                index->line     = bfx->loops[i].end.line;
                index->line_idx = bfx->loops[i].end.line_idx;
            }
        }
    }
}

/**
 * @brief End of loop (brainfuck).
 */
void bfx_op_brainfuck_loop_end(BFX* bfx, BFX_FileIndex* index) {
    if (bfx->tape[bfx->tp]) {
        for (size_t i = 0; i < bfx->loops_len; i++) {
            if (bfx->loops[i].end.idx == bfx->ip) {
                bfx->ip         = bfx->loops[i].start.idx;
                index->line     = bfx->loops[i].start.line;
                index->line_idx = bfx->loops[i].start.line_idx;
            }
        }
    }
}

/**
 * @brief Get character from input or stdin (brainfuck).
 */
void bfx_op_brainfuck_getchar(BFX* bfx, BFX_FileIndex* index) {
    char c;
    if (bfx->flags & BFX_FLAG_SEPARATE_INPUT_AND_SOURCE) {
        if (bfx->input_ptr < bfx->input_len) {
            c = bfx->program[bfx->input_ptr];
            bfx->input_ptr++;
        } else {
            c = EOF;
        }
    } else {
        c = fgetc(stdin);
    }

    if (bfx->receiving) {
        if (c == EOF) {
            bfx->receiving = false;
        } else {
            bfx->tape[bfx->tp] = c;
        }
    }

    if (!bfx->receiving) {
        switch (bfx->eof_behavior) {
        case BFX_EOF_BEHAVIOR_ZERO:
            bfx->tape[bfx->tp] = 0;
            break;
        case BFX_EOF_BEHAVIOR_DECREMENT:
            bfx->tape[bfx->tp]--;
            break;
        }
    }
}

/**
 * @brief Output the current cell value as a character (brainfuck).
 */
void bfx_op_brainfuck_putchar(BFX* bf, BFX_FileIndex* index) { putchar(bf->tape[bf->tp]); }
