/**
 * @file langs/weave.h
 *
 * This file contains function definitions related to interpreting the Weave language.
 */
#ifndef BFX_LANGS_WEAVE_H
#define BFX_LANGS_WEAVE_H

#include "bfx.h"

BFX_Error bfx_weave_init(BFX*);
BFX_Error bfx_weave_run(BFX*);

/* ops */
BFX_Error bfx_op_weave_toggle(BFX*, BFX_FileIndex*);
BFX_Error bfx_op_weave_exit(BFX*, BFX_FileIndex*);

#endif
