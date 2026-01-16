#ifndef BFX_PBRAIN_H
#define BFX_PBRAIN_H

#include "../bfx.h"

typedef struct {
    char   identifier;
    size_t start_idx;
    size_t end_idx;
} bfx_pbrain_procedure_t;

typedef struct {
    bfx_pbrain_procedure_t* procedures;
    size_t              procedures_len;
    size_t*             stack;
    size_t              stack_top;

} bfx_pbrain_data_t;

void bfx_pbrain_init(bfx_t* bfx);
void bfx_pbrain_run(bfx_t* bf);

#endif
