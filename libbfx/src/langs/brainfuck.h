/**
 * @file langs/bf.c
 *
 * This file contains public definitions for the implementation of the Brainfuck interpreter.
 */

#ifndef BFX_LANGS_BF_H

#define BFX_LANGS_BF_H

#include "bfx.h"

BFX_Error bfx_brainfuck_init(BFX*);
BFX_Error bfx_brainfuck_run(BFX*);

/* ops */
BFX_Error bfx_op_brainfuck_inc_tp(BFX*, BFX_FileIndex*);
BFX_Error bfx_op_brainfuck_dec_tp(BFX*, BFX_FileIndex*);
BFX_Error bfx_op_brainfuck_inc_t(BFX*, BFX_FileIndex*);
BFX_Error bfx_op_brainfuck_dec_t(BFX*, BFX_FileIndex*);
BFX_Error bfx_op_brainfuck_loop_start(BFX*, BFX_FileIndex*);
BFX_Error bfx_op_brainfuck_loop_end(BFX*, BFX_FileIndex*);
BFX_Error bfx_op_brainfuck_getchar(BFX*, BFX_FileIndex*);
BFX_Error bfx_op_brainfuck_putchar(BFX*, BFX_FileIndex*);

#endif
