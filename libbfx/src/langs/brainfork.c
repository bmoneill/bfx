#include "brainfork.h"

#include "brainfuck.h"
#include "util.h"

#include <string.h>

static int (*bfx_brainfork_ops[128])(BFX*, BFX_FileIndex*) = {
    [']'] = bfx_op_brainfuck_loop_end, ['['] = bfx_op_brainfuck_loop_start,
    ['+'] = bfx_op_brainfuck_inc_t,    ['-'] = bfx_op_brainfuck_dec_t,
    ['>'] = bfx_op_brainfuck_inc_tp,   ['<'] = bfx_op_brainfuck_dec_tp,
    [','] = bfx_op_brainfuck_getchar,  ['.'] = bfx_op_brainfuck_putchar,
    ['Y'] = bfx_op_brainfork_fork,
};

/**
 * @brief Initialize the Brainfork interpreter
 * @param bfx Pointer to the already-allocated interpreter struct
 */
void bfx_brainfork_init(BFX* bfx) {
    bfx->lang_data = calloc(1, sizeof(BFX_BrainforkData));
    bfx_build_loops(bfx);
}

/**
 * @brief Run the Brainfork interpreter
 * @param bfx Pointer to the interpreter struct
 */
void bfx_brainfork_run(BFX* bfx) {
    bfx_parse_ops(bfx, bfx_brainfork_ops);

    BFX_BrainforkData* data = bfx->lang_data;
    for (size_t i = 0; i < data->thread_count; i++) {
        pthread_join(data->threads[i], NULL);
    }
}

/**
 * @brief Run the Brainfork interpreter in a child thread
 * @param arg Pointer to the interpreter struct
 */
void* bfx_brainfork_run_child(void* arg) {
    BFX* bfx = (BFX*) arg;
    bfx_parse_ops(bfx, bfx_brainfork_ops);
    free(bfx);
    return NULL;
}

/**
 * @brief Fork (brainfork)
 */
int bfx_op_brainfork_fork(BFX* bfx, BFX_FileIndex* index) {
    BFX_BrainforkData* data = bfx->lang_data;
    if (data->thread_count >= BFX_MAX_THREADS) {
        BFX_ERROR("Maximum thread count reached");
        return 1;
    }

    BFX* childBFX = malloc(sizeof(BFX));
    memcpy(childBFX, bfx, sizeof(BFX));
    childBFX->tape = calloc(bfx->tape_size, sizeof(uint8_t));
    memcpy(childBFX->tape, bfx->tape, bfx->tape_size);

    bfx->tape[bfx->tp] = 0;
    childBFX->tp++;
    childBFX->tape[childBFX->tp] = 1;
    childBFX->ip++;

    pthread_create(&data->threads[data->thread_count++], NULL, bfx_brainfork_run_child, childBFX);
    return 0;
}
