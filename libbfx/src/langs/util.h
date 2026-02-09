/**
 * @file langs/util.h
 * @brief Language-independent utility function definitions.
 */
#ifndef BFX_LANGS_UTIL_H
#define BFX_LANGS_UTIL_H

#include "bfx.h"

void bfx_parse_ops(BFX*, void (*[128])(BFX*, BFX_FileIndex*));

#endif
