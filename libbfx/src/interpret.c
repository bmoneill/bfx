/**
 * @file interpret.c
 *
 * This file contains the implementation of the Brainfuck-like language interpreter.
 */

#include "interpret.h"
#include "bfx.h"
#include "langs/bf.h"
#include "langs/pbrain.h"
#include "langs/weave.h"

void bfx_interpret(BFX* bfx) {
    switch (bfx->lang) {
    case BFX_LANG_BRAINFUCK:
        bfx_bf_init(bfx);
        bfx_bf_run(bfx);
        break;
    case BFX_LANG_PBRAIN:
        bfx_pbrain_init(bfx);
        bfx_pbrain_run(bfx);
        break;
    case BFX_LANG_BRAINFORK:
        break; // TODO implement
    case BFX_LANG_WEAVE:
        bfx_weave_init(bfx);
        bfx_weave_run(bfx);
        break;
    case BFX_LANG_GRIN:
        break; // TODO implement
    }
}
