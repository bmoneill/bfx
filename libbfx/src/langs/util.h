/**
 * @file langs/util.h
 * @brief Language-independent utility function definitions.
 */
#ifndef BFX_LANGS_UTIL_H
#define BFX_LANGS_UTIL_H

#include "bfx.h"

#ifndef BFX_MAX_THREADS
/**
 * @brief Maximum number of threads allowed in multithreaded languages.
 */
#define BFX_MAX_THREADS 128
#endif

BFX_Error bfx_build_loops(BFX*);
BFX_Error bfx_parse_ops(BFX*, BFX_Error (*[128])(BFX*, BFX_FileIndex*));

#endif
