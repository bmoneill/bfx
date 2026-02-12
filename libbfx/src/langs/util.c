/**
 * @file langs/util.c
 * @brief Language-independent utility functions.
 */
#include "util.h"

/**
 * @brief Builds the loop structure for the BFX instance.
 *
 * This function scans the BFX program and constructs the necessary
 * data structures to efficiently handle loop constructs ('[' and ']').
 * It ensures that matching brackets are correctly paired, allowing for
 * proper execution flow during interpretation.
 *
 * @param bfx The BFX instance to build loops for.
 *
 * @note This function does not return a value. If an error occurs (such as exceeding MAX_LOOPS),
 *       it prints an error message and terminates the program using exit(EXIT_FAILURE).
 */
void bfx_build_loops(BFX* bfx) {
    BFX_FileIndex* stack      = malloc(sizeof(BFX_FileIndex) * BFX_INITIAL_LOOP_SIZE);
    int            stack_top  = 0;
    int            stack_size = BFX_INITIAL_LOOP_SIZE;
    int            line       = 1;
    int            line_idx   = 0;
    bfx->loops_len            = 0;
    bfx->loops_size           = BFX_INITIAL_LOOP_SIZE;
    bfx->loops                = malloc(sizeof(BFX_Block) * BFX_INITIAL_LOOP_SIZE);

    for (size_t i = 0; i < bfx->program_len; i++) {
        line_idx++;
        if (bfx->program[i] == '[') {
            if (stack_top >= stack_size) {
                stack_size *= 2;
                stack = realloc(stack, sizeof(BFX_FileIndex) * stack_size);
            }
            stack[stack_top].idx      = i;
            stack[stack_top].line     = line;
            stack[stack_top].line_idx = line_idx;
            stack_top++;
        } else if (bfx->program[i] == ']') {
            if (stack_top <= 0) {
                fprintf(stderr,
                        "libbfx: Error (%d,%d): Unmatched closing bracket ']'.\n",
                        line,
                        line_idx);
                free(stack);
                exit(EXIT_FAILURE);
            }
            BFX_FileIndex start                     = stack[--stack_top];
            bfx->loops[bfx->loops_len].start        = start;
            bfx->loops[bfx->loops_len].end.idx      = i;
            bfx->loops[bfx->loops_len].end.line     = line;
            bfx->loops[bfx->loops_len].end.line_idx = line_idx;
            bfx->loops_len++;
        } else if (bfx->program[i] == '\n') {
            line++;
            line_idx = 0;
        }
    }

    free(stack);

    if (stack_top != 0) {
        fprintf(stderr, "libbfx: Error (%d,%d): Unmatched opening bracket '['.\n", line, line_idx);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Executes operations on the BFX instance.
 *
 * @param bfx The BFX instance to run
 * @param ops Operation map
 */
void bfx_parse_ops(BFX* bfx, int (*ops[128])(BFX*, BFX_FileIndex*)) {
    BFX_FileIndex idx;
    idx.idx      = 0;
    idx.line_idx = 0;
    idx.line     = 1;

    while ((size_t) bfx->ip < bfx->program_len) {
        if (bfx->program[bfx->ip] == '\n') {
            idx.line++;
            idx.line_idx = 0;
        } else {
            idx.line_idx++;
        }

        int op = (int) bfx->program[bfx->ip];
        if (ops[op]) {
            int ret = ops[(int) bfx->program[bfx->ip]](bfx, &idx);
            if (ret) {
                fprintf(stderr,
                        "libbfx: Error (%d,%d): Operation failed.\n",
                        idx.line,
                        idx.line_idx);
                return;
            }
        }
        bfx->ip++;
    }
}
