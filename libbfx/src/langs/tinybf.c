#include "tinybf.h"

#include "util.h"

#include <stdlib.h>

#define TINYBF_LEFT  -1
#define TINYBF_RIGHT 1

/**
 * @brief Initialize the Brainfuck interpreter
 * @param bfx Pointer to the already-allocated interpreter struct
 */
void bfx_tinybf_init(BFX* bfx) { bfx->lang_data = malloc(sizeof(uint8_t)); }

/**
 * @brief Run the Brainfuck interpreter
 * @param bfx Pointer to the interpreter struct
 */
void bfx_tinybf_run(BFX* bfx) {
    void (*ops[128])(BFX*, BFX_FileIndex*) = {
        ['='] = bfx_op_tinybf_switch_direction,
        ['+'] = bfx_op_tinybf_change_t,
        ['>'] = bfx_op_tinybf_change_tp,
        ['|'] = bfx_op_tinybf_jump,
    };
    bfx_parse_ops(bfx, ops);
}

void bfx_op_tinybf_switch_direction(BFX* bfx, BFX_FileIndex* idx) {
    int* direction = ((int*) bfx->lang_data);
    if (*direction == TINYBF_LEFT) {
        *direction = TINYBF_RIGHT;
    } else {
        *direction = TINYBF_LEFT;
    }
}

void bfx_op_tinybf_change_t(BFX* bfx, BFX_FileIndex* idx) {
    int* direction = ((int*) bfx->lang_data);
    bfx->tape[bfx->tp] += *direction;
}

void bfx_op_tinybf_change_tp(BFX* bfx, BFX_FileIndex* idx) {
    int* direction = ((int*) bfx->lang_data);
    bfx->tp += *direction;
}

void bfx_op_tinybf_jump(BFX* bfx, BFX_FileIndex* idx) {
    // TODO Implement
}
