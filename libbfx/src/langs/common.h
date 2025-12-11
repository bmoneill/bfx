#ifndef BFX_OPS_COMMON_H
#define BFX_OPS_COMMON_H

#include "../bfx.h"

typedef struct {
    char identifier;
    void* (*execute)(bfx_t*, bfx_file_index_t*);
} bfx_op_t;

#endif
