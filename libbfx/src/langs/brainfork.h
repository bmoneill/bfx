#ifndef BFX_LANGS_BRAINFORK_H
#define BFX_LANGS_BRAINFORK_H

#include "../bfx.h"
#include "util.h"

#include <pthread.h>

/**
 * @brief Data structure for managing Brainfork threads
 */
typedef struct {
    pthread_t threads[BFX_MAX_THREADS]; //!< Threads
    size_t    thread_count; //!< Number of threads
} BFX_BrainforkData;

void  bfx_brainfork_init(BFX*);
void  bfx_brainfork_run(BFX*);
void* bfx_brainfork_run_child(void*);

/* ops */
void bfx_op_brainfork_fork(BFX*, BFX_FileIndex*);

#endif
