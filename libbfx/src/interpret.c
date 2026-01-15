#include "interpret.h"
#include "bfx.h"
#include "langs/bf.h"

void bfx_interpret(bfx_t* bfx) {
    switch (bfx->lang) {
    case BFX_LANG_BRAINFUCK:
        bfx_bf_run(bfx);
        break;
    }
}
