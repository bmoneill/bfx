/**
 * @file interpret.c
 *
 * This file contains the implementation of the Brainfuck-like language interpreter.
 */

#include "interpret.h"
#include "bfx.h"

#define BFX_LANG(s)                                                                                \
    case BFX_LANG_##s:                                                                             \
        ret = bfx_##s##_init(bfx);                                                                 \
        if (ret) {                                                                                 \
            return ret;                                                                            \
        }                                                                                          \
        return bfx_##s##_run(bfx);

BFX_Error bfx_interpret(BFX* bfx) {
    int ret;
    switch (bfx->lang) {
        BFX_LANG(brainfuck)
        BFX_LANG(brainfork)
        BFX_LANG(grin)
        BFX_LANG(pbrain)
        BFX_LANG(weave)
    case BFX_LANG_UNKNOWN:
        BFX_ERROR("Unknown language");
        return BFX_RUNTIME_ERROR;
    }

    return BFX_RUNTIME_ERROR;
}
