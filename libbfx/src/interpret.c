#include "interpret.h"
#include "bfx.h"
#include "langs/bf.h"
#include "langs/pbrain.h"

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
    }
}
