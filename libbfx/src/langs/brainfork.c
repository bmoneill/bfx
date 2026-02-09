#include "brainfork.h"

#include "brainfuck.h"
#include "util.h"

#include <pthread.h>
#include <stdlib.h>

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
    void (*ops[128])(BFX*, BFX_FileIndex*) = {
        [']'] = bfx_op_loop_end, ['['] = bfx_op_loop_start, ['+'] = bfx_op_inc_t,
        ['-'] = bfx_op_dec_t,    ['>'] = bfx_op_inc_tp,     ['<'] = bfx_op_dec_tp,
        [','] = bfx_op_getchar,  ['.'] = bfx_op_putchar,    ['Y'] = bfx_op_fork,
    };
    bfx_parse_ops(bfx, ops);
}

/**
 * @brief Run the Brainfork interpreter in a child thread
 * @param arg Pointer to the interpreter struct
 */
void* bfx_brainfork_run_child(void* arg) {
    BFX* bfx = (BFX*) arg;
    bfx_brainfork_run(bfx);
    return NULL;
}

/**
 * @brief Fork (brainfork)
 */
void bfx_op_fork(BFX* bfx, BFX_FileIndex* index) {
    BFX_BrainforkData* data = bfx->lang_data;
    pthread_create(&data->threads[data->thread_count++], NULL, bfx_brainfork_run_child, bfx);
}
