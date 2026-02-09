/**
 * @file langs/weave.c
 *
 * This file contains functions related to interpreting the Weave language.
 */

#include "weave.h"

#include "bfx.h"
#include "ops.h"
#include "util.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Weave language data structure.
 */
typedef struct {
    BFX*   threads; //!< Pointer to the array of threads
    size_t threadCount; //!< Number of threads
} BFX_WeaveData;

/**
 * @brief Weave thread wrapper structure.
 *
 * Used in bfx_weave_create() for pthread_create().
 */
typedef struct {
    BFX* bfx; //!< Pointer to the main BFX interpreter structure
    void (*ops[128])(BFX*, BFX_FileIndex*); //!< Pointer to the array of operations
} BFX_WeaveWrapper;

static void* bfx_weave_create(void*);

/**
 * @brief Initialize the Weave interpreter
 * @param bfx Pointer to the already-allocated interpreter struct
 */
void bfx_weave_init(BFX* bfx) {
    int    threadCount = 0;
    size_t threadLengths[BFX_WEAVE_MAX_THREADS];

    threadLengths[0] = 0;
    for (size_t i = 0; i < bfx->program_len; i++) {
        if (bfx->program[i] == ';') {
            if (threadCount > BFX_WEAVE_MAX_THREADS) {
                BFX_ERROR("Too many threads.");
            }
            threadCount++;
            threadLengths[threadCount] = 0;
        } else {
            threadLengths[threadCount]++;
        }
    }

    int progIdx              = 0;
    BFX_WeaveData* weaveData = malloc(sizeof(BFX_WeaveData));
    bfx->lang_data           = weaveData;
    weaveData->threads       = calloc(threadCount + 1, sizeof(BFX));
    weaveData->threadCount   = threadCount + 1;

    for (size_t i = 0; i < weaveData->threadCount; i++) {
        BFX* threadBFX          = &weaveData->threads[i];
        threadBFX->program      = malloc(threadLengths[i] + 1);
        threadBFX->program_len  = threadLengths[i];
        threadBFX->program_size = threadLengths[i];

        memcpy(threadBFX->program, bfx->program + progIdx, threadLengths[i]);
        progIdx += threadLengths[i];

        // Thread lang_data is a pointer to the main tape (switched with bfx_op_toggle)
        threadBFX->tape      = calloc(1, BFX_DEFAULT_TAPE_SIZE);
        threadBFX->tape_size = BFX_DEFAULT_TAPE_SIZE;
        threadBFX->lang_data = bfx->tape;

        bfx_build_loops(threadBFX);
    }
}

/**
 * @brief Run the Weave interpreter
 *
 * This function initializes and runs the Weave interpreter threads.
 * It creates and joins the threads, and frees the language-specific data.
 *
 * @param bfx Pointer to the interpreter struct
 */
void bfx_weave_run(BFX* bfx) {
    void (*ops[128])(BFX*, BFX_FileIndex*) = {
        [']'] = bfx_op_loop_end, ['['] = bfx_op_loop_start, ['+'] = bfx_op_inc_t,
        ['-'] = bfx_op_dec_t,    ['>'] = bfx_op_inc_tp,     ['<'] = bfx_op_dec_tp,
        [','] = bfx_op_getchar,  ['.'] = bfx_op_putchar,    ['~'] = bfx_op_toggle,
    };

    BFX_WeaveData* data    = (BFX_WeaveData*) bfx->lang_data;
    pthread_t*     threads = malloc(sizeof(pthread_t) * data->threadCount);
    for (size_t i = 0; i < data->threadCount; i++) {
        BFX*              threadBFX = &data->threads[i];
        BFX_WeaveWrapper* wrapper   = malloc(sizeof(BFX_WeaveWrapper));
        wrapper->bfx                = threadBFX;
        memcpy(wrapper->ops, ops, sizeof(ops));
        pthread_create(&threads[i], NULL, bfx_weave_create, wrapper);
    }

    for (size_t i = 0; i < data->threadCount; i++) {
        pthread_join(threads[i], NULL);
    }

    // Free language-specific data
    free(threads);
    for (size_t i = 0; i < data->threadCount; i++) {
        free(data->threads[i].program);
        if (data->threads[i].tape == bfx->tape) {
            free(data->threads[i].lang_data);
        } else {
            free(data->threads[i].tape);
        }
        free(data->threads[i].loops);
    }
    free(data);
}

/**
 * @brief Thread entry point for Weave interpreter threads
 *
 * @param data Pointer to the Weave wrapper struct
 * @return NULL
 */
static void* bfx_weave_create(void* data) {
    BFX_WeaveWrapper* wrapper = (BFX_WeaveWrapper*) data;
    BFX*              bfx     = wrapper->bfx;
    bfx_parse_ops(bfx, wrapper->ops);
    return NULL;
}
