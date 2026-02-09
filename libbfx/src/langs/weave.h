#ifndef BFX_OPS_WEAVE_H
#define BFX_OPS_WEAVE_H

#include "../bfx.h"

#ifndef BFX_WEAVE_MAX_THREADS
#define BFX_WEAVE_MAX_THREADS 128
#endif

void bfx_weave_init(BFX*);
void bfx_weave_run(BFX*);

#endif
