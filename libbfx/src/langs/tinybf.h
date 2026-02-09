/**
 * @file langs/tinybf.h
 *
 * This file contains function definitions related to interpreting the TinyBF language.
 */
#ifndef BFX_LANGS_TINYBF_H
#define BFX_LANGS_TINYBF_H

#include "../bfx.h"

void bfx_tinybf_init(BFX*);
void bfx_tinybf_init(BFX*);

/* ops */
void bfx_op_tinybf_switch_direction(BFX*, BFX_FileIndex*);
void bfx_op_tinybf_change_t(BFX*, BFX_FileIndex*);
void bfx_op_tinybf_change_tp(BFX*, BFX_FileIndex*);
void bfx_op_tinybf_jump(BFX*, BFX_FileIndex*);

#endif
