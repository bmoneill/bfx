/**
 * @file bfx.c
 * @brief Main logic for the brainfuck-like language interpreter/compiler.
 */

#include "bfx.h"

#include "interpret.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void bfx_init_tokens(void);
static int  bfx_load_file(BFX*, const char*);
static void bfx_reset_loops(BFX*);

/**
 * @brief Diagnoses the BFX instance.
 *
 * This function prints the current state of the BFX instance, including the line number,
 * tape pointer, instruction pointer, and memory map.
 *
 * @param bfx The BFX instance to diagnose.
 * @param idx The file index to use for line information.
 */
void bfx_diagnose(BFX* bfx, BFX_FileIndex* idx) {
    fprintf(stderr,
            "Line: %d,%d\nTape pointer: %d\nInstruction pointer: %d\n",
            idx->line,
            idx->line_idx,
            bfx->tp,
            bfx->ip);

    fprintf(stderr, "Memory map:\n");
    for (int i = 0; i < bfx->tp_max; i++) {
        fprintf(stderr, "%d: %d\n", i, bfx->tape[i]);
    }
}

/**
 * @brief Runs the program loaded from a file.
 *
 * This function builds the loop structure for the BFX instance,
 * then iterates through the program instructions, interpreting each one
 * until the end of the program is reached.
 */
BFX_Error bfx_run_file(const char* path, BFX* bfx) {
    if (!bfx_load_file(bfx, path)) {
        return BFX_RUNTIME_ERROR;
    }
    return bfx_interpret(bfx);
}

/**
 * @brief Runs the BFX interpreter in REPL (Read-Eval-Print Loop) mode.
 *
 * This function continuously reads input from the user, appends it to the program,
 * and interprets the instructions until the user terminates the program.
 */
BFX_Error bfx_run_repl(BFX* bfx) {
    char* input;
    bfx->program_size = bfx->input_max;
    int ret           = BFX_SUCCESS;

    if (!(bfx->program = (char*) malloc(bfx->program_size + 1))
        || !(input = (char*) malloc(bfx->program_size + 1))) {
        BFX_ERROR("Cannot allocate memory for program storage.");
        return BFX_RUNTIME_ERROR;
    }

    while (1) {
        printf("> ");
        if (!fgets(input, bfx->input_max, stdin)) {
            break;
        }

        size_t prog_len_old = bfx->program_len;
        bfx->program_len += strlen(input);
        if (bfx->program_len > bfx->program_size) {
            bfx->program_size *= 2;
            if (!(bfx->program = realloc(bfx->program, bfx->program_size))) {
                BFX_ERROR("Cannot reallocate memory for program storage.");
                return BFX_RUNTIME_ERROR;
            }
        }

        snprintf(bfx->program + prog_len_old, bfx->program_size - prog_len_old, "%s", input);

        ret = bfx_interpret(bfx);
        if (ret) {
            break;
        }
    }

    free(input);
    input = NULL;
    bfx_free(bfx);
    return ret;
}

/**
 * @brief Frees the memory allocated for the BFX instance.
 *
 * This function frees the memory allocated for the BFX instance, including the program buffer,
 * tape buffer, and loop buffer. It does not free the BFX instance itself.
 *
 * @param bfx Pointer to the BFX instance.
 */
void bfx_free(BFX* bfx) {
    if (bfx) {
        if (bfx->program) {
            free(bfx->program);
            bfx->program = NULL;
        }
        if (bfx->tape) {
            free(bfx->tape);
            bfx->tape = NULL;
        }
        if (bfx->loops) {
            free(bfx->loops);
            bfx->loops = NULL;
        }
    }
}

/**
 * @brief Loads source code from a file.
 *
 * This function opens the specified file, reads its contents into a dynamically allocated buffer,
 * and stores the program length. If the file cannot be opened or memory allocation fails,
 * it prints an error message and returns a non-zero value to indicate failure.
 *
 * @param bfx Pointer to the BFX instance.
 * @param path The path to the source file.
 *
 * @return Returns 0 on success, or 1 if an error occurs (e.g., file not found, memory allocation failure).
 *
 * @note The program is expected to be in plain text format, with compatible instructions.
 *       The function allocates memory for the program and reads the entire file into this buffer.
 *       The caller is responsible for freeing the allocated memory.
 */
static int bfx_load_file(BFX* bfx, const char* path) {
    FILE* f;
    if ((f = fopen(path, "r"))) {
        fseek(f, 0, SEEK_END);
        bfx->program_len = ftell(f);
        fseek(f, 0, SEEK_SET);

        if ((bfx->program = (char*) malloc(bfx->program_len))) {
            if (fread(bfx->program, 1, bfx->program_len, f) != (unsigned long) bfx->program_len) {
                fprintf(stderr, "Error: Cannot read file %s.\n", path);
                free(bfx->program);
                bfx->program = NULL;
                fclose(f);
                return 1;
            }
        } else {
            fprintf(stderr, "Error: Cannot allocate memory for program storage.\n");
            fclose(f);
            return 1;
        }
        fclose(f);
    } else {
        fprintf(stderr, "Error: Cannot open file %s for reading.\n", path);
        return 1;
    }

    if (bfx->flags & BFX_FLAG_SEPARATE_INPUT_AND_SOURCE) {
        for (size_t i = 0; i < bfx->program_len; i++) {
            if (bfx->program[i] == '!') {
                bfx->input_start = i + 1;
                bfx->input_ptr   = bfx->input_start;
                bfx->input_len   = bfx->program_len;
                bfx->program[i]  = '\0';
                bfx->program_len = i;
            }
        }
    }

    return 0;
}
