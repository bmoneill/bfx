/**
 * @file langs/util.c
 * @brief Language-independent utility functions.
 */
#include "util.h"

/**
 * @brief Executes operations on the BFX instance.
 *
 * @param bfx The BFX instance to run
 * @param ops Operation map
 */
void bfx_parse_ops(BFX* bfx, void (*ops[128])(BFX*, BFX_FileIndex*)) {
    BFX_FileIndex idx;
    idx.idx      = 0;
    idx.line_idx = 0;
    idx.line     = 1;

    while ((size_t) bfx->ip < bfx->program_len) {
        if (bfx->program[bfx->ip] == '\n') {
            idx.line++;
            idx.line_idx = 0;
        } else {
            idx.line_idx++;
        }

        int op = (int) bfx->program[bfx->ip];
        if (ops[op]) {
            ops[(int) bfx->program[bfx->ip]](bfx, &idx);
        }
        bfx->ip++;
    }
}
