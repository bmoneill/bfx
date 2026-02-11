/**
 * @file langs/weave.h
 *
 * This file contains function definitions related to interpreting the Weave language.
 */
#ifndef BFX_LANGS_WEAVE_H
#define BFX_LANGS_WEAVE_H

#include "bfx.h"

void bfx_weave_init(BFX*);
void bfx_weave_run(BFX*);

/* ops */
void bfx_op_weave_toggle(BFX*, BFX_FileIndex*);

#endif
