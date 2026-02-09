/**
 * @file langs/weave.h
 *
 * This file contains function definitions related to interpreting the Weave language.
 */
#ifndef BFX_LANGS_WEAVE_H
#define BFX_LANGS_WEAVE_H

#include "../bfx.h"

#ifndef BFX_WEAVE_MAX_THREADS
/**
 * @brief Maximum number of threads allowed in the Weave language.
 */
#define BFX_WEAVE_MAX_THREADS 128
#endif

void bfx_weave_init(BFX*);
void bfx_weave_run(BFX*);

#endif
