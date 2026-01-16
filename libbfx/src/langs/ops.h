#ifndef BFX_OPS_H
#define BFX_OPS_H

#include "../bfx.h"

/* brainfuck */
void bfx_op_inc_tp(BFX*, BFX_FileIndex*);
void bfx_op_dec_tp(BFX*, BFX_FileIndex*);
void bfx_op_inc_t(BFX*, BFX_FileIndex*);
void bfx_op_dec_t(BFX*, BFX_FileIndex*);
void bfx_op_loop_start(BFX*, BFX_FileIndex*);
void bfx_op_loop_end(BFX*, BFX_FileIndex*);
void bfx_op_getchar(BFX*, BFX_FileIndex*);
void bfx_op_putchar(BFX*, BFX_FileIndex*);

/* pbrain */
void bfx_op_call(BFX*, BFX_FileIndex*);
void bfx_op_ret(BFX*, BFX_FileIndex*);

#endif
