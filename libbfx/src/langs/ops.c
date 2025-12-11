#include "ops.h"

#include "bfx.h"
#include "pbrain.h"

#include <stdio.h>

/**
 * @brief Increment tape pointer (brainfuck).
 */
void op_inc_tp(bfx_t* bf, bfx_file_index_t* index) {
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
void op_dec_tp(bfx_t* bf, bfx_file_index_t* index) {
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
void op_inc_t(bfx_t* bf) { bf->tape[bf->tp]++; }

/**
 * @brief Decrement tape value (brainfuck).
 */
void op_dec_t(bfx_t* bf) { bf->tape[bf->tp]--; }

/**
 * @brief Start of loop (brainfuck).
 */
void op_loop_start(bfx_t* bf, bfx_file_index_t* index) {
    size_t i;
    if (!bf->tape[bf->tp]) {
        for (i = 0; i < bf->loops_len; i++) {
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
void op_loop_end(bfx_t* bf, bfx_file_index_t* index) {
    size_t i;
    if (bf->tape[bf->tp]) {
        for (i = 0; i < bf->loops_len; i++) {
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
void op_getchar(bfx_t* bf, bfx_file_index_t* index) {
    char c;
    if (bf->flags & BFX_FLAG_SEPARATE_INPUT_AND_SOURCE) {
        if (bf->input_ptr < bf->input_len) {
            c = bf->prog[bf->input_ptr];
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
void op_putchar(bfx_t* bf, bfx_file_index_t* index) { putchar(bf->tape[bf->tp]); }

/**
 * @brief Call a procedure by its identifier (pbrain).
 */
void op_call(bfx_t* bf, bfx_file_index_t* index) {
    size_t         i;
    pbrain_data_t* data = (pbrain_data_t*) bf->lang_data;
    for (i = 0; i < data->procedures_len; i++) {
        if (data->procedures[i].identifier == bf->tape[bf->tp]) {
            data->stack_top++;
            data->stack[data->stack_top] = bf->ip;
            bf->ip                       = data->procedures[i].start_idx;
            return;
        }
    }
}

void op_ret(bfx_t* bf) {
    pbrain_data_t* data = (pbrain_data_t*) bf->lang_data;
    if (data->stack_top > 0) {
        bf->ip = data->stack[data->stack_top];
        data->stack_top--;
    }
}
