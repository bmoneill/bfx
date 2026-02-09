#include "tinybf.h"

#include "../bfx.h"

#include <stdlib.h>

/**
 * @brief Initialize the Brainfuck interpreter
 * @param bfx Pointer to the already-allocated interpreter struct
 */
void bfx_tinybf_init(BFX* bfx) {
    bfx->lang_data = malloc(sizeof(uint8_t));
}

/**
 * @brief Run the Brainfuck interpreter
 * @param bfx Pointer to the interpreter struct
 */
void bfx_tinybf_run(BFX* bfx) {
    void (*ops[128])(BFX*, BFX_FileIndex*) = {
        ['='] = bfx_op_switch_direction, ['+'] = bfx_op_change_t, ['>'] = bfx_op_change_tp,
        ['-'] = bfx_op_jump,    ['>'] = bfx_op_inc_tp,     ['<'] = bfx_op_dec_tp,
        [','] = bfx_op_getchar,  ['.'] = bfx_op_putchar,
    };
    bfx_parse_ops(bfx, ops);
}
