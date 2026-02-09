#include "weave.h"

#include "bf.h"
#include "ops.h"
#include "util.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    BFX*   threads;
    size_t threadCount;
} BFX_WeaveData;

typedef struct {
    BFX* bfx;
    void (*ops[128])(BFX*, BFX_FileIndex*);
} BFX_WeaveWrapper;

static void* bfx_weave_create(void*);

/**
 * @brief Initialize the Weave interpreter
 * @param bfx Pointer to the already-allocated interpreter struct
 */
void bfx_weave_init(BFX* bfx) {
    int    threadIdx = 0;
    size_t threadLengths[BFX_WEAVE_MAX_THREADS];

    for (size_t i = 0; i < bfx->program_len; i++) {
        if (bfx->program[i] == ';') {
            threadIdx = 0;
            if (threadIdx > BFX_WEAVE_MAX_THREADS) {
                BFX_ERROR("Too many threads.");
            }
            threadLengths[threadIdx] = 0;
        } else {
            threadLengths[threadIdx]++;
        }
    }

    int progIdx              = 0;
    bfx->lang_data           = malloc(sizeof(BFX_WeaveData));

    BFX_WeaveData* weaveData = (BFX_WeaveData*) bfx->lang_data;
    weaveData->threads       = malloc(sizeof(BFX) * threadIdx + 1);
    weaveData->threadCount   = threadIdx + 1;

    threadIdx                = 0;
    for (size_t i = 0; i < weaveData->threadCount; i++) {
        BFX* threadBFX          = &((BFX*) bfx->lang_data)[i];
        threadBFX->program      = malloc(threadLengths[i]);
        threadBFX->program_len  = threadLengths[i];
        threadBFX->program_size = threadBFX->program_len;
        memcpy(threadBFX->program, bfx->program + progIdx, threadLengths[i]);
        progIdx += threadLengths[i];

        // Thread lang_data is a pointer to the main tape (switched with bfx_op_toggle)
        threadBFX->tape      = calloc(1, BFX_DEFAULT_TAPE_SIZE);
        threadBFX->tape_size = BFX_DEFAULT_TAPE_SIZE;
        threadBFX->lang_data = bfx->tape;

        bfx_bf_init(threadBFX);
    }
}

/**
 * @brief Run the Weave interpreter
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

    // Cleanup
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

static void* bfx_weave_create(void* data) {
    BFX_WeaveWrapper* wrapper = (BFX_WeaveWrapper*) data;
    BFX*              bfx     = wrapper->bfx;
    bfx_parse_ops(bfx, wrapper->ops);
    return NULL;
}
