#ifndef BFX_OPS_H
#define BFX_OPS_H

#include "../bfx.h"

void bfx_op_inc_tp(bfx_t*, bfx_file_index_t*);
void bfx_op_dec_tp(bfx_t*, bfx_file_index_t*);
void bfx_op_inc_t(bfx_t*, bfx_file_index_t*);
void bfx_op_dec_t(bfx_t*, bfx_file_index_t*);
void bfx_op_loop_start(bfx_t*, bfx_file_index_t*);
void bfx_op_loop_end(bfx_t*, bfx_file_index_t*);
void bfx_op_getchar(bfx_t*, bfx_file_index_t*);
void bfx_op_putchar(bfx_t*, bfx_file_index_t*);

#endif
